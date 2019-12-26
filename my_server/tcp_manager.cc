#include "tcp_manager.h"

TcpManager::TcpManager(string local_ip, int16_t local_port)
: local_ip_(local_ip), local_port_(local_port)
{
}

TcpManager::~TcpManager(void)
{
}

int TcpManager::on_msg(InnerMsg msg)
{
    switch (msg.msg_id_)
    {
        case TCP_MANAGER_REACTOR_REGISTER:
        {
            TCP_CONNECTOR_STATE_MAP tcp_state;
            reactors_[msg.handler_] = tcp_state;
        } break;
        case TCP_MANAGER_REACTOR_LOGOUT:
        {
            auto iter = reactors_.find(msg.handler_);
            if (iter != reactors_.end()) {
                reactors_.erase(msg.handler_);
            }
        } break;
        case TCP_MANAGER_RECV_DATA:
        {
            net_packet_.push_recv_packet(msg.buffer_);
        } break;
        case TCP_MANAGER_SOCK_FD:
        {
            TcpState tmp;
            tmp.socket_fd_ = msg.param1_;
            reactors_[msg.handler_][tmp.socket_fd_] = tmp;
        } break;
        default:
        {

        } break;
    }
}
int TcpManager::run_handler()
{
    while(!exit_) {
        if (net_packet_.parse_recv_packet() == NETPACKET_INCOMPLETE) {
            usleep(200);
        }
    }
}

int TcpManager::exit_handler()
{
    exit_ = true;
}


int TcpManager::connect_to(string ip, int16_t port)
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);

    int ret = ::connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (ret == -1) {
        return -1;
    }

    InnerMsg msg(REACTOR_ADD_FD, object_id_, ret);
    MSGObject::trans_msg(reactors_.begin()->first, msg);

    return 0;
}

int TcpManager::shutdown_connect(int sockfd)
{
    InnerMsg msg(REACTOR_ADD_FD, object_id_, sockfd);
    MSGObject::trans_msg(reactors_.begin()->first, msg);

    reactors_.begin()->second.erase(sockfd);
    usleep(500); // 等待reactor中删除sockfd
    
    ::shutdown(sockfd, SHUT_RDWR);

    return 0;
}
int TcpManager::server_listening(void)
{
    int listen_fd;
    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(local_port_);
    inet_pton(AF_INET, local_ip_.c_str(), &servaddr.sin_addr);

    int ret = ::bind(listen_fd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (ret == -1) {
        return -1;
    }

    ret = ::listen(listen_fd, 5);
    if (ret == -1) {
        return -1;
    }

    InnerMsg msg(REACTOR_ADD_FD, object_id_, ret, 1); // param2 == 1 表示 listenfd
    MSGObject::trans_msg(reactors_.begin()->first, msg);

    return 0;
}

int TcpManager::ret_client_socket_fd(void)
{
    // 怎么及时通知上级有客户端连接
}
int TcpManager::push_data(pair<int, shared_ptr<Buffer>> &ret);
int TcpManager::get_data(pair<int, shared_ptr<Buffer>> &ret);
OBJ_HANDLE find_reactor(int sockfd);