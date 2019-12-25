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
    case TCP_MANAGER_RECV_DATA:
    {
        net_packet_.push_recv_packet(msg.buffer_);
    } break;
    case TCP_MANAGER_SOCK_FD:
    {
        TcpState tmp;
        tmp.socket_fd_ = msg.param1_;
        tcp_states_[tmp.socket_fd_] = tmp;
    } break;
    default:
        break;
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

    int ret = connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (ret == -1) {
        return -1;
    }

    InnerMsg msg();
}

int TcpManager::showdown_connect(int fd);
int TcpManager::server_listening(void);
int TcpManager::ret_client_socket_fd(void);
int TcpManager::push_data(pair<int, shared_ptr<Buffer>> &ret);
int TcpManager::get_data(pair<int, shared_ptr<Buffer>> &ret);