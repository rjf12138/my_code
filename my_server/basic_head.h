#ifndef __BASIC_HEAD_H__
#define __BASIC_HEAD_H__

/////////////// c++ header file //////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <map>
#include <memory>
#include <queue>

////////////// linux system header file ////////////////

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

////////////// defined by myself ////////////////////////

#include "error.h"
#include "buffer.h"
#include "queue.h"
#include "thread.h"
#include "mutex.h"
#include "server_define.h"
#include "inner_msg_object.h"

#endif