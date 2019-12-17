#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/*
*   消息的基本结构(头部总长度不应该超过1600字节，消息采用网络字节序)
*   magic_num   (1个字节头(用 $， 其它地方要用到这两个字符需要进行替代))
*   消息序号     （2个字节[单个消息体序号为0，多个组合消息，对这两个字节分为两部分，消息类型+消息编号]）
*   消息体长度   (2个字节[取决于消息体的长度][头部长度+消息体长度+校验码])
*   消息体      (未知)
*/

/*
*   消息体解析取决于具体的消息类
*/
#include "basic_head.h"

#define MAX_MSG_LENGTH          1480
#define MAX_MSG_BODY_LENGTH     1473 // 减去消息的7个包字节
#define MSG_HEAD_START          '$'
#define MSG_HEAD_LENS           5

class Message {
public:
    Message() {}
    virtual ~Message() {}

    int check_msg(void);
    Buffer ret_msg_body(void); // 返回消息体
    int add_msg_data(Buffer &msg_data);

public:
    uint16_t msg_num_; // 消息编号在这个类内生成
    uint16_t msg_len_;
    Buffer msg_buf_; // 保存一则字符串消息
    Queue<Buffer> msg_body_queue_;  // 消息体队列
};

int
Message::check_msg(void)    // 检查消息是否完成，或出错
{
    int start_pos = msg_buf_.get_start_pos();
    int end_pos = msg_buf_.get_end_pos();
    int8_t* buff = msg_buf_.get_buffer();

    if (buff[start_pos] != MSG_HEAD_START) {
        return ERROR_PARSE_MESSAGE;
    }

    if (msg_buf_.data_size() < MSG_HEAD_LENS) {
        return MSG_INCOMPLETE;
    }

    int8_t double_bytes[2] = {0};
    double_bytes[0] = buff[msg_buf_.get_next_pos(start_pos)];
    double_bytes[1] = buff[msg_buf_.get_next_pos(start_pos+1)];
    memcpy(&msg_num_, double_bytes, 2); 
    msg_num_ = htons(msg_num_);

    double_bytes[0] = buff[msg_buf_.get_next_pos(start_pos+2)];
    double_bytes[1] = buff[msg_buf_.get_next_pos(start_pos+3)];
    memcpy(&msg_len_, double_bytes, 2); 
    msg_len_ = htons(msg_len_);

    if (msg_len_ > MAX_MSG_LENGTH) {
        return ERROR_PARSE_MESSAGE;
    }

    if (msg_buf_.data_size() < msg_len_) {
        return MSG_INCOMPLETE;
    }

    Buffer tmp(MAX_MSG_LENGTH);
    tmp.copy_to_buffer(msg_buf_, start_pos + 5, msg_len_);
    msg_body_queue_.push(tmp);

    return 0;
}

Buffer
Message::ret_msg_body(void)
{
   Buffer ret;
   msg_body_queue_.pop(ret);

   return ret;
}

int 
Message::add_msg_data(Buffer &msg_data)
{
    return msg_buf_.copy_to_buffer(msg_data, msg_data.get_start_pos(), msg_data.data_size());
}


////////////////////// Inner Msg Struct //////////////////////////

#include "inner_msg_object.h"

struct InnerMsg {
    int msg_id_;
    OBJ_HANDLE handler_;
    int param1_;
    int param2_;
    string str_;
    shared_ptr<void> object_;

    InnerMsg(int msg_id = 0, OBJ_HANDLE handler = 0, int param1 = 0, int param2 = 0, string str = "", void* object = nullptr):
        msg_id_(msg_id), handler_(handler), param1_(param1), param2_(param2),str_(str), object_(object)
    {}
};

///////////////////////////// 自定义消息 ////////////////////////////
#endif