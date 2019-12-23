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

class TcpManager {
public:
    TcpManager(string local_ip, int16_t local_port);
    virtual ~TcpManager(void);

    // 向远端发起 tcp 连接,成功时返回sockfd
    int connect_to(string ip, int16_t port);
    // 创建服务端监听的socket_fd
    int server_listening(void);
    // 客户端主动连接成功时，返回sockfd
    int ret_client_socket_fd(void);

    int push_data(void);
    int get_data(void);
private:
    string local_ip_;
    int16_t local_port_;
    Reactor reactor_;
};

#endif