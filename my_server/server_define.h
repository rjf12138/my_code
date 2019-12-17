#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__

enum EVENT_HANDLER {
    TYPE_LISTEN_FD = 1000,
    TYPE_NETWORK_FD = 1001,
    TYPE_PIPE_FD = 1002,
    TYPE_FILE_FD = 1003,
};

enum MESSAGE {
    MSG_INCOMPLETE = 1050,
    MSG_COMPLETE = 1051
};


////////////////////// inner_msg_id /////////////////////////

enum REACTOR_MSG_ID {
    MSG_ADD_FD = 11000,
};

enum MSG_HANDER_MSG_ID {
    MSG_DATA_BUFFER = 11500,
};

#endif