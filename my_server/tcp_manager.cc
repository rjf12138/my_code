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
        shared_ptr<Buffer> recv_data = static_cast<shared_ptr<Buffer>>(msg.object_);
        net_packet_.push_recv_packet(msg.object_);
    } break;
    case TCP_MANAGER_SOCK_FD:
    {

    } break;
    default:
        break;
    }
}
int TcpManager::run_handler() ;
int TcpManager::exit_handler() ;