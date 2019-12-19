#ifndef __NetPacket_H__
#define __NetPacket_H__

/*
*   消息的基本结构(头部总长度不应该超过1600字节，消息采用网络字节序)
*   magic_num   (1个字节头(用 $， 其它地方要用到这两个字符需要进行替代))
*   消息包编号  （2个字节，对于每一个链接的，每一个独立的消息都会有一个独立的编号）
*   消息包个数  （2个字节[当消息大于一个包的长度时会被分包，包的总个数被保存在这]）
*   消息体长度   (2个字节[取决于消息体的长度][头部长度+消息体长度+校验码])
*   消息体      (未知)
*/

/*
*   消息体解析取决于具体的消息类
*/
#include "basic_head.h"

#define MAX_PACKET_LENGTH          1480
#define MAX_PACKET_BODY_LENGTH     1473 // 减去消息的7个包字节
#define PACKET_HEAD_START          '$'
#define PACKET_HEAD_LENS           7

struct PacketInfo {
    // Msg 以存在时间超过最大寿命包将被丢弃
    uint16_t packet_life;
    // 消息目前状态
    uint16_t packet_state;
    // 消息编号: 标志每一个消息包
    uint16_t packet_len;
    // 当前已经读到的数据长度
    uint16_t curr_len;
    // 消息包个数： 用于分包时标志该消息分为多少个包
    uint16_t packet_num;
    // 消息编号： 用于标志一个消息
    uint16_t packet_identity;
    // 消息缓冲
    shared_ptr<Buffer> msg_buff;
};

class NetPacket {
public:
    NetPacket();
    virtual ~NetPacket();

    int push_recv_packet_packet(Buffer &buff);
    int parse_recv_packet_packet(void);

    int get_packet_head(shared_ptr<PacketInfo> &packet_info_ptr);
    shared_ptr<PacketInfo> get_packet_info(int packet_num);

public:
    // 包处理的状态
    uint16_t packet_handle_state_;
    // 当前处理包的编号
    uint16_t curr_packet_num_;
    // 缓存消息
    Buffer packet_buf_;
    // 保存收到的消息体
    map<uint16_t, shared_ptr<PacketInfo>> packet_in_map_;
    // 保存要发送的消息
    Queue<shared_ptr<Buffer>> msg_out_queue_;
};

///////////////////////////// 自定义消息 ////////////////////////////
#endif