#include "net_packet.h"


NetPacket::NetPacket()
: packet_handle_state_(NETPACKET_HANDLE_IDLE)
{

}
NetPacket::~NetPacket() 
{

}

int 
NetPacket::push_recv_packet_packet(Buffer &buff)
{
    if (packet_buf_.full()) {
        return ERROR_NETPACKET_BUF_IS_FULL;
    }

    packet_buf_.copy_to_buffer(buff, 0, buff.data_size());

    return 0;
}

int
NetPacket::parse_recv_packet_packet(void)
{
    if (packet_buf_.data_size() < PACKET_HEAD_LENS) {
        return NETPACKET_INCOMPLETE;
    }

    switch(packet_handle_state_) {
        case NETPACKET_HANDLE_IDLE: 
        {
            shared_ptr<PacketInfo> packet_info_ptr = this->get_packet_info(-1);
            if (this->get_packet_head(packet_info_ptr) != ERROR_PARSE_NETPACKET) {
                this->get_packet_head(packet_info_ptr);
                packet_handle_state_ = NETPACKET_HANDLE_MSG;
                curr_packet_num_ = packet_info_ptr->packet_num;
            }
        } break;
        case NETPACKET_HANDLE_MSG:
        {
            shared_ptr<PacketInfo> packet_info_ptr = this->get_packet_info(curr_packet_num_);
            uint16_t start_pos = packet_buf_.get_start_pos();
            uint16_t read_size = packet_buf_.data_size() <= packet_info_ptr->packet_len ?
                                        packet_buf_.data_size() : packet_info_ptr->packet_len;
            packet_info_ptr->msg_buff->copy_to_buffer(packet_buf_, start_pos, read_size);
            packet_info_ptr->curr_len += read_size;
            if (packet_info_ptr->curr_len >= packet_info_ptr->packet_len) {
                msg_out_queue_.push(packet_info_ptr->msg_buff);
            }
        } break;
        default:
        {

        }
    }

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
NetPacket::get_packet_info(int packet_num)
{
    auto iter = packet_in_map_.find(packet_num);
    if (iter == packet_in_map_.end()) {
        packet_in_map_[packet_num] = make_shared<PacketInfo>();
    }

    return packet_in_map_[packet_num];
}

// int
// NetPacket::check_packet(void)
// {
//     int start_pos = packet_buf_.get_start_pos();
//     int end_pos = packet_buf_.get_end_pos();
//     int8_t* buff = packet_buf_.get_buffer();

//     if (buff[start_pos] != PACKET_HEAD_START) {
//         return ERROR_PARSE_NetPacket;
//     }

//     if (packet_buf_.data_size() < PACKET_HEAD_LENS) {
//         return PACKET_INCOMPLETE;
//     }

//     int8_t double_bytes[2] = {0};
//     double_bytes[0] = buff[packet_buf_.get_next_pos(start_pos)];
//     double_bytes[1] = buff[packet_buf_.get_next_pos(start_pos+1)];
//     memcpy(&packet_num_, double_bytes, 2); 
//     packet_num_ = htons(packet_num_);

//     double_bytes[0] = buff[packet_buf_.get_next_pos(start_pos+2)];
//     double_bytes[1] = buff[packet_buf_.get_next_pos(start_pos+3)];
//     memcpy(&packet_len_, double_bytes, 2); 
//     packet_len_ = htons(packet_len_);

//     if (packet_len_ > MAX_PACKET_LENGTH) {
//         return ERROR_PARSE_NetPacket;
//     }

//     if (packet_buf_.data_size() < packet_len_) {
//         return PACKET_INCOMPLETE;
//     }

//     shared_ptr<Buffer> ptr = get_new_packet_buff(packet_num_);
//     ptr->copy_to_buffer(packet_buf_, start_pos + PACKET_HEAD_LENS, packet_len_);

//     return 0;
// }

// shared_ptr<Buffer>
// NetPacket::get_new_packet_buff(uint16_t packet_num)
// {
//     auto iter = packet_in_map_.find(packet_num);
//     if (iter == packet_in_map_.end()) {
//         packet_in_map_[packet_num] = make_shared<Buffer>();
//     }

//     return packet_in_map_[packet_num];
// }

// int
// NetPacket::ret_packet_body(Buffer &ret)
// {
//    return packet_body_queue_.pop(ret);
// }

// int 
// NetPacket::add_packet_data(Buffer &packet_data)
// {
//     return packet_buf_.copy_to_buffer(packet_data, packet_data.get_start_pos(), packet_data.data_size());
// }
