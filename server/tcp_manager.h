#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__


/*
*   无论是主动监听还是被动的，每条连接都会生成一个独一无二的编号
*   需要返回每个sockfd给上级，供上级自己去和对应的客户对接
*/
#include "basic_head.h"
#include "reactor.h"
#include "net_packet.h"

struct TcpState {
    int socket_fd_;
    int16_t port_;
    string ip_;
};

typedef map<int, TcpState> TCP_CONNECTOR_STATE_MAP;
typedef map<OBJ_HANDLE, TCP_CONNECTOR_STATE_MAP> REACTOR_MAP;

class TcpManager : public MSGObject, public Thread{
public:
    TcpManager(OBJ_HANDLE hander,string local_ip, int16_t local_port);
    virtual ~TcpManager(void);

    virtual int on_msg(InnerMsg msg) override;
    virtual int run_handler() override;
    virtual int exit_handler() override;

    int start(void);
    int stop(void);
    // 向远端发起 tcp 连接,成功时返回sockfd
    int connect_to(string ip, int16_t port);
    // 关闭一条tcp连接
    int shutdown_connect(int fd);
    // 创建服务端监听的socket_fd
    int server_listening(void);
    // 客户端主动连接成功时，返回sockfd
    int ret_client_socket_fd(void);
    // 添加要发送的消息
    int push_data(int sockfd,  shared_ptr<Buffer> &data);

private:
    OBJ_HANDLE find_reactor(int sockfd);

private:
    bool exit_ = false;
    string local_ip_;
    int16_t local_port_;
    OBJ_HANDLE hander_;
    NetPacket net_packet_;
    REACTOR_MAP reactors_; // 目前只支持加入一个 reactor
};

#endif