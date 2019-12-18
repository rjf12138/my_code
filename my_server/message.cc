#include "message.h"


int 
Message::push_recv_msg_packet(Buffer &buff)
{
    int start_pos = buff.get_start_pos();
    int end_pos = buff.get_end_pos();
    int8_t *buff_ptr = buff.get_buffer();

    if (buff_ptr[start_pos] != MSG_HEAD_START) {
        return ERROR_PARSE_MESSAGE;
    }

    if (msg_buf_.data_size() < MSG_HEAD_LENS) {
        return MSG_INCOMPLETE;
    }
}

// int
// Message::check_msg(void)
// {
//     int start_pos = msg_buf_.get_start_pos();
//     int end_pos = msg_buf_.get_end_pos();
//     int8_t* buff = msg_buf_.get_buffer();

//     if (buff[start_pos] != MSG_HEAD_START) {
//         return ERROR_PARSE_MESSAGE;
//     }

//     if (msg_buf_.data_size() < MSG_HEAD_LENS) {
//         return MSG_INCOMPLETE;
//     }

//     int8_t double_bytes[2] = {0};
//     double_bytes[0] = buff[msg_buf_.get_next_pos(start_pos)];
//     double_bytes[1] = buff[msg_buf_.get_next_pos(start_pos+1)];
//     memcpy(&msg_num_, double_bytes, 2); 
//     msg_num_ = htons(msg_num_);

//     double_bytes[0] = buff[msg_buf_.get_next_pos(start_pos+2)];
//     double_bytes[1] = buff[msg_buf_.get_next_pos(start_pos+3)];
//     memcpy(&msg_len_, double_bytes, 2); 
//     msg_len_ = htons(msg_len_);

//     if (msg_len_ > MAX_MSG_LENGTH) {
//         return ERROR_PARSE_MESSAGE;
//     }

//     if (msg_buf_.data_size() < msg_len_) {
//         return MSG_INCOMPLETE;
//     }

//     shared_ptr<Buffer> ptr = get_new_msg_buff(msg_num_);
//     ptr->copy_to_buffer(msg_buf_, start_pos + MSG_HEAD_LENS, msg_len_);

//     return 0;
// }

// shared_ptr<Buffer>
// Message::get_new_msg_buff(uint16_t msg_num)
// {
//     auto iter = msg_in_map_.find(msg_num);
//     if (iter == msg_in_map_.end()) {
//         msg_in_map_[msg_num] = make_shared<Buffer>();
//     }

//     return msg_in_map_[msg_num];
// }

// int
// Message::ret_msg_body(Buffer &ret)
// {
//    return msg_body_queue_.pop(ret);
// }

// int 
// Message::add_msg_data(Buffer &msg_data)
// {
//     return msg_buf_.copy_to_buffer(msg_data, msg_data.get_start_pos(), msg_data.data_size());
// }
