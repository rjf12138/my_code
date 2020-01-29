#include "net_packet.h"
#include "error.h"


NetPacket::NetPacket(void)
: next_packet_identity_(0)
{

}
NetPacket::~NetPacket(void) 
{

}

int 
NetPacket::push_recv_packet(Buffer &buff)
{
    if (packet_buf_.full()) {
        return ERROR_NETPACKET_BUF_IS_FULL;
    }

    packet_buf_.copy_to_buffer(buff, 0, buff.data_size());

    return 0;
}

int
NetPacket::parse_recv_packet(void)
{
    switch(packet_handle_state_) {
        case NETPACKET_HANDLE_IDLE: 
        {
            if (packet_buf_.data_size() < PACKET_HEAD_LENS) {
                return NETPACKET_INCOMPLETE;
            }

            shared_ptr<PacketInfo> ptr = make_shared<PacketInfo>();
            if (this->get_packet_head(ptr) != ERROR_PARSE_NETPACKET) {
                shared_ptr<PacketInfo> packet_info_ptr = this->get_packet_info(ptr->packet_identity);
                *packet_info_ptr = *ptr;

                packet_handle_state_ = NETPACKET_HANDLE_MSG;
                curr_packet_identity_ = packet_info_ptr->packet_identity;

                if (packet_info_ptr->msg_buff.size() <= 0) { 
                    int size = packet_info_ptr->entire_msg_len / MAX_PACKET_BODY_LENGTH;
                    int packet_frame_size = packet_info_ptr->entire_msg_len % MAX_PACKET_BODY_LENGTH == 0 ? size : size + 1;
                    packet_info_ptr->msg_buff.reserve(packet_frame_size);
                }
            }
        } break;
        case NETPACKET_HANDLE_MSG:
        {
            shared_ptr<PacketInfo> packet_info_ptr = this->get_packet_info(curr_packet_identity_);
            uint16_t curr_packet_frame = packet_info_ptr->packet_frame_identity;
            uint16_t start_pos = packet_buf_.get_start_pos();

            uint16_t remain_size = packet_info_ptr->packet_len - packet_info_ptr->curr_packet_len;
            uint16_t read_size = packet_buf_.data_size() <= remain_size ?
                                        packet_buf_.data_size() : remain_size;
            packet_info_ptr->msg_buff[curr_packet_frame]->copy_to_buffer(packet_buf_, start_pos, read_size);
            packet_info_ptr->curr_packet_len += read_size;

            if (packet_info_ptr->curr_packet_len >= packet_info_ptr->packet_len) {
                packet_handle_state_ = NETPACKET_HANDLE_IDLE;
                packet_info_ptr->curr_packet_len = 0; // 重新设置当前包长为0

                // 当所有消息都收到是就将消息组合之后返回
                packet_info_ptr->curr_msg_len += packet_info_ptr->packet_len;
                if (packet_info_ptr->curr_msg_len >= packet_info_ptr->entire_msg_len) {
                    shared_ptr<Buffer> ret = this->merge_all_msg_frame(*packet_info_ptr);
                    msg_in_queue_.push(ret);
                    packet_in_map_.erase(packet_info_ptr->packet_identity);

                    return NETPACKET_COMPLETE;
                }
            }
        } break;
        default:
        {
        }
    }

    return NETPACKET_INCOMPLETE;
}

int 
NetPacket::push_send_msg(shared_ptr<Buffer> &buff)
{
    PacketInfo packet_info;
    packet_info.entire_msg_len = buff->data_size();
    packet_info.packet_identity = this->get_next_packet_identity();

    int size = buff->data_size() / MAX_PACKET_BODY_LENGTH;
    int packet_frame_size = buff->data_size() % MAX_PACKET_BODY_LENGTH == 0 ? size : size + 1;

    shared_ptr<Buffer> msg_buf = make_shared<Buffer>();
    int curr_write_len = 0;
    for (int i = 0; i < packet_frame_size; ++i) {
        int write_size = (i == packet_frame_size - 1) ? 
                            buff->data_size() - curr_write_len : MAX_PACKET_BODY_LENGTH;

        packet_info.packet_frame_identity = i;
        packet_info.packet_len = write_size;
        curr_write_len += write_size;
        this->generate_packet_head(msg_buf, packet_info);
        msg_buf->copy_to_buffer(*buff, buff->get_start_pos(), buff->data_size());
        msg_out_queue_.push(msg_buf);
    }
    return 0;
}

int 
NetPacket::get_msg_frame(shared_ptr<Buffer> &frame)
{
    int ret = msg_out_queue_.pop(frame);
    if (ret != -1) {
        return 0;
    }

    return -1;
}

int
NetPacket::get_packet_head(shared_ptr<PacketInfo> &packet_info_ptr)
{
    int8_t magic_num = 0;
    packet_buf_.read_int8(magic_num);
    if (magic_num != PACKET_HEAD_START) {
        return ERROR_PARSE_NETPACKET;
    }

    packet_buf_.read_int16_ntoh((int16_t&)packet_info_ptr->packet_identity);
    packet_buf_.read_int16_ntoh((int16_t&)packet_info_ptr->entire_msg_len);
    packet_buf_.read_int16_ntoh((int16_t&)packet_info_ptr->packet_len);
    packet_buf_.read_int16_ntoh((int16_t&)packet_info_ptr->packet_frame_identity);

    return 0;
}

shared_ptr<PacketInfo>
NetPacket::get_packet_info(int packet_identity)
{
    auto iter = packet_in_map_.find(packet_identity);
    if (iter == packet_in_map_.end()) {
        packet_in_map_[packet_identity] = make_shared<PacketInfo>();
    }

    return packet_in_map_[packet_identity];
}

int 
NetPacket::get_msg_from_queue(shared_ptr<Buffer> &msg_body)
{
    return msg_in_queue_.pop(msg_body);
}

uint16_t 
NetPacket::get_next_packet_identity(void)
{
    return next_packet_identity_++ % MAX_PACKET_NUM;
}

int 
NetPacket::generate_packet_head(shared_ptr<Buffer> &msg, PacketInfo &packet_info)
{
    msg->write_int8(PACKET_HEAD_START);
    msg->write_int16(packet_info.packet_identity);
    msg->write_int16(packet_info.entire_msg_len);
    msg->write_int16(packet_info.packet_len);
    msg->write_int16(packet_info.packet_frame_identity);

    return 0;
}

shared_ptr<Buffer> 
NetPacket::merge_all_msg_frame(PacketInfo &packet_info)
{
    vector<shared_ptr<Buffer>> &data = packet_info.msg_buff;
    shared_ptr<Buffer> in_msg = make_shared<Buffer>();
    for (int i = 0; i < data.size(); ++i) {
        in_msg->copy_to_buffer(*data[i], data[i]->get_start_pos(), data[i]->data_size());
    }

    return in_msg;
}