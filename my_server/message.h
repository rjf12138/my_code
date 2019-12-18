#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/*
*   消息的基本结构(头部总长度不应该超过1600字节，消息采用网络字节序)
*   magic_num   (1个字节头(用 $， 其它地方要用到这两个字符需要进行替代))
*   消息序号     （2个字节[单个消息体序号为0，多个组合消息，对这两个字节分为两部分，消息个数+消息编号]）
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

struct MsgPacket {
    // Msg 以存在时间超过最大寿命包将被丢弃
    uint16_t msg_life;
    // 消息目前状态
    uint16_t msg_state;
    // 消息编号: 标志每一个消息包 |消息个数|消息编号|
    uint16_t msg_len;
    // 消息包个数： 用于分包时标志该消息分为多少个包，从消息序号中获取
    uint16_t msg_packet_num;
    // 消息编号： 用于标志一个消息，从消息序号中获取
    uint16_t msg_identity;
    // 消息缓冲
    shared_ptr<Buffer> msg_buff;
};

class Message {
public:
    Message():{}
    virtual ~Message() {}

    int push_recv_msg_packet(Buffer &buff);
    int parse_recv_msg_packet(void);
    // // 检查消息缓存，是否有完整的消息存在
    // int check_msg(void);
    // // 返回已解包的消息体
    // Buffer ret_msg_body(void);
    // MsgPacket get_new_msg_buff(uint16_t msg_num)
    // // 添加消息到消息缓存中
    // int add_msg_data(Buffer &msg_data);

    // int push_msg_to_buff()；

public:
    uint16_t msg_num_;
    // 消息长度
    uint16_t msg_len_;
    // 缓存消息
    Buffer msg_buf_;
    // 保存收到的消息体
    map<uint16_t, MsgPacket> msg_in_map_;
    // 保存要发送的消息
    Queue<Buffer> msg_out_queue_;
};

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