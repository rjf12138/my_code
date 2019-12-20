#include "net_packet.h"


NetPacket::NetPacket()
: packet_handle_state_(NETPACKET_HANDLE_IDLE),
  next_packet_identity_(0)
{

}
NetPacket::~NetPacket() 
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
            shared_ptr<PacketInfo> packet_info_ptr = this->get_packet_info(-1);
            if (this->get_packet_head(packet_info_ptr) != ERROR_PARSE_NETPACKET) {
                packet_handle_state_ = NETPACKET_HANDLE_MSG;
                curr_packet_identity_ = packet_info_ptr->packet_identity;
                packet_info_ptr->msg_buff.reserve(packet_info_ptr->len / );
            }
        } break;
        case NETPACKET_HANDLE_MSG:
        {
            // 每个分包需要有自己的编号， 当分包来时会可能会是乱序的，因此需要根据分包编号来
            // 重新构建消息， 需要修改代码
            shared_ptr<PacketInfo> packet_info_ptr = this->get_packet_info(curr_packet_identity_);
            uint16_t curr_packet_frame = packet_info_ptr->packet_num;
            uint16_t start_pos = packet_buf_.get_start_pos();
            uint16_t remain_size = packet_info_ptr->packet_len - packet_info_ptr->curr_len;
            uint16_t read_size = packet_buf_.data_size() <= remain_size ?
                                        packet_buf_.data_size() : remain_size;
            packet_info_ptr->msg_buff[curr_packet_frame]->copy_to_buffer(packet_buf_, start_pos, read_size);
            packet_info_ptr->curr_len += read_size;

            if (packet_info_ptr->curr_len >= packet_info_ptr->packet_len) {
                for (auto iter = )
                packet_in_map_.erase(curr_packet_identity_);
                packet_handle_state_ = NETPACKET_HANDLE_IDLE;
            }
        } break;
        default:
        {
        }
    }

    return NETPACKET_INCOMPLETE;
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
    packet_buf_.read_int16_ntoh((int16_t&)packet_info_ptr->packet_num);
    packet_buf_.read_int16_ntoh((int16_t&)packet_info_ptr->packet_len);

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
NetPacket::get_next_packet_num(void)
{
    return next_packet_identity_++ % MAX_PACKET_NUM;
}

int NetPacket::push_send_msg(Buffer &buff)
{

}