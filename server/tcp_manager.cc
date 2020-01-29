#include "tcp_manager.h"

TcpManager::TcpManager(OBJ_HANDLE hander,string local_ip, int16_t local_port)
: hander_(hander), local_ip_(local_ip), local_port_(local_port)
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
            net_packet_.push_recv_packet(*msg.buffer_);
        } break;
        case TCP_MANAGER_SOCK_FD:
        {
            TcpState tmp;
            tmp.socket_fd_ = msg.param1_;
            reactors_[msg.handler_][tmp.socket_fd_] = tmp;

            InnerMsg msg(APP_NEW_CLIENT_FD, object_id_, tmp.socket_fd_);
            this->trans_msg(hander_, msg);
        } break;
        default:
        {

        } break;
    }
}
int TcpManager::run_handler()
{
    while(!exit_) {
        int ret = net_packet_.parse_recv_packet();
        if ( ret == NETPACKET_INCOMPLETE) {
            usleep(200);
        } else if (ret == NETPACKET_COMPLETE) {
            InnerMsg msg(APP_NEW_CLIENT_MSG, object_id_);
            net_packet_.get_msg_from_queue(msg.buffer_);
            this->trans_msg(hander_, msg);
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
    // 通知reactor加入新的sock_fd
    InnerMsg msg(REACTOR_ADD_FD, object_id_, ret);
    MSGObject::trans_msg(reactors_.begin()->first, msg);
    // 通知上级保存新的sock_fd
    InnerMsg msg(APP_NEW_CLIENT_FD, hander_, ret);
    this->trans_msg(hander_, msg);

    return 0;
}

int TcpManager::shutdown_connect(int sockfd)
{
    InnerMsg msg(REACTOR_ADD_FD, object_id_, sockfd);
    MSGObject::trans_msg(reactors_.begin()->first, msg);

    reactors_.begin()->second.erase(sockfd);
    ::usleep(500); // 等待reactor中删除sockfd
    
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

    InnerMsg msg(REACTOR_ADD_FD, object_id_, ret, TCP_MANAGER_LISTEND_FD);
    MSGObject::trans_msg(reactors_.begin()->first, msg);

    return 0;
}

int TcpManager::push_data(int sockfd, shared_ptr<Buffer> &data)
{
    net_packet_.push_send_msg(data);

    shared_ptr<Buffer> frame_ptr;
    while (net_packet_.get_msg_frame(frame_ptr) != -1) {
        // send_msg
    }
    

}

OBJ_HANDLE find_reactor(int sockfd) { /* TODO */}