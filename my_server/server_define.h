#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__

enum EVENT_HANDLER {
    TYPE_LISTEN_FD = 1000,
    TYPE_NETWORK_FD = 1001,
    TYPE_PIPE_FD = 1002,
    TYPE_FILE_FD = 1003,
};

enum NETPACKET {
    NETPACKET_INCOMPLETE = 1050,
    NETPACKET_COMPLETE = 1051,
    // 包处理状态
    NETPACKET_HANDLE_IDLE = 1052, // 空闲
    NETPACKET_HANDLE_MSG = 1053, // 正在处理消息
};

enum TCP_MANAGER {
    TCP_MANAGER_RECV_DATA,
    TCP_MANAGER_SOCK_FD,
};

////////////////////// inner_msg_id /////////////////////////

enum REACTOR_MSG_ID {
    MSG_ADD_FD = 11000,
};

enum MSG_HANDER_MSG_ID {
    MSG_DATA_BUFFER = 11500,
};

#endif