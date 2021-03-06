#include "reactor.h"

Reactor::Reactor(OBJ_HANDLE manager_hander): 
    manager_hander_(manager_hander) 
{ }

Reactor::~Reactor(void) 
{
    if (epoll_fd_ != -1) {
        ::close(epoll_fd_);
    }
}

int Reactor::init(void)
{
    // 5 只是提示内核表要多大，并不起作用
    epoll_fd_ = ::epoll_create(5);
    return epoll_fd_;
}

int Reactor::run_handler(void)
{
    while(!exit_) {
        int ret = ::epoll_wait(epoll_fd_, event_, MAX_EVENT_NUM, 5000);
        if (ret == -1) {
            ::sleep(1);
            continue;
        }

        for (int i = 0; i < MAX_EVENT_NUM; ++i) {
            this->handler_event(event_[i].data.fd);
        }
    }
}

int Reactor::exit_handler(void)
{
    exit_ = true;
}

int Reactor::on_msg(InnerMsg msg)
{
    switch (msg.msg_id_) {
        case REACTOR_ADD_FD:
        {
            if (msg.param2_ == TCP_MANAGER_LISTEND_FD) {
                listen_fd_ = msg.param1_;
                this->add_fd(listen_fd_);
            }
        } break;
        case REACTOR_DEL_FD:
        {

        } break;
        default:
        {

        } break;
    }
}

int Reactor::add_fd(int fd, bool is_listen_fd = false)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    ::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);

    if (is_listen_fd == true) {
        listen_fd_ = fd;
    }

    return 0;
}

int Reactor::del_fd(int fd) 
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    ::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &event);

    return 0;
}

int Reactor::setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    ::fcntl(fd, F_SETFL, new_option);

    return old_option;
}

int Reactor::handler_event(int fd)
{
    if (fd == listen_fd_) {
        struct sockaddr_in client_struct;
        socklen_t client_len = sizeof(client_struct);

        int client_fd = accept(fd, (sockaddr*)&client_struct, &client_len);
        if (client_fd == -1) {
            return -1;
        }

        this->add_fd(client_fd);
        InnerMsg msg(TCP_MANAGER_SOCK_FD, object_id_, fd);
        this->trans_msg(manager_hander_, msg);
    } else {
        char buf[MAX_MSG_BUF] = {0};
        int len = recv(fd, buf, MAX_MSG_BUF, 0);
        if (len == -1) {
            return -1;
        }

        shared_ptr<Buffer> buff_ptr = make_shared<Buffer>();
        buff_ptr->write_bytes(buf, len);
        InnerMsg msg(TCP_MANAGER_RECV_DATA, object_id_, len, fd);
        msg.set_buffer(buff_ptr);
        this->trans_msg(manager_hander_, msg);
        // 直接发到 connector 里
    }

    return 0;
}