#ifndef __NetPacket_H__
#define __NetPacket_H__

/*
*   消息的基本结构(头部总长度不应该超过1600字节，消息采用网络字节序)
*   magic_num   (1个字节头(用 $， 其它地方要用到这两个字符需要进行替代))
*   消息编号  （2个字节，对于每一个链接的，每一个独立的消息都会有一个独立的编号）
*   完整消息长度   (2个字节 消息可能会分包)
*   消息分包长度 （2个字节）
*   消息分包编号   (2个字节)
*   消息体      (未知)
*/

/*
*   消息体解析取决于具体的消息类
*/
#include "basic_head.h"

#define MAX_PACKET_LENGTH          1480
#define PACKET_HEAD_LENS           9
#define MAX_PACKET_BODY_LENGTH     1471 // 减去消息的7个包字节
#define PACKET_HEAD_START          '$'
#define MAX_PACKET_NUM             65535
#define MAX_PACKET_LIFE            300

struct PacketInfo {
public:
    // Msg 以存在时间超过最大寿命包将被丢弃
    uint16_t packet_life;
    // 消息目前状态
    uint16_t packet_state;
    // 当前已经读到总的的数据长度
    uint16_t curr_msg_len;
    // 当前读到包的消息体长度
    uint16_t curr_packet_len;
    // 整个消息长度（不包含消息头）
    uint16_t entire_msg_len;
    // 消息分包的长度 （不包含消息头）
    uint16_t packet_len;
    // 消息分包序号： 标志一个消息内分包的序号
    uint16_t packet_frame_identity;
    // 消息编号： 用于标志一个消息
    uint16_t packet_identity;
    // 消息缓冲
    vector<shared_ptr<Buffer>> msg_buff;

public:
    PacketInfo& operator=(PacketInfo &lval)
    {
        this->packet_life = lval.packet_life;
        this->packet_state = lval.packet_state;
        this->packet_len = lval.packet_len;
        this->packet_frame_identity = lval.packet_frame_identity;
        this->packet_identity = lval.packet_identity;

        return *this;
    }
};

class NetPacket {
public:
    NetPacket();
    virtual ~NetPacket();

    int push_recv_packet(Buffer &buff);
    int parse_recv_packet(void);
    int get_msg_from_queue(shared_ptr<Buffer> &msg_body);

    int push_send_msg(Buffer &buff);
    
    uint16_t get_next_packet_identity(void);
    int generate_packet_head(shared_ptr<Buffer> &msg, PacketInfo &packet_info);
    int get_packet_head(shared_ptr<PacketInfo> &packet_info_ptr);
    shared_ptr<PacketInfo> get_packet_info(int packet_identity);
    shared_ptr<Buffer> merge_all_msg_frame(PacketInfo &packet_info);

public:
    // 要发送的包编号
    uint16_t next_packet_identity_;
    // 包处理的状态
    uint16_t packet_handle_state_;
    // 当前处理包的编号
    uint16_t curr_packet_identity_;
    // 缓存消息
    Buffer packet_buf_;
    // 保存收到的消息体
    map<uint16_t, shared_ptr<PacketInfo>> packet_in_map_;
    // 保存要发送的消息
    Queue<shared_ptr<Buffer>> msg_in_queue_;
    Queue<shared_ptr<Buffer>> msg_out_queue_;
};

///////////////////////////// 自定义消息 ////////////////////////////
#endif