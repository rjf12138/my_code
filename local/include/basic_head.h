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
#include <utility>

using namespace std;
////////////// linux system header file ////////////////

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

////////////// defined by myself ////////////////////////

#include "error.h"
#include "buffer.h"
#include "queue.h"
#include "thread.h"
#include "mutex.h"

#endif
