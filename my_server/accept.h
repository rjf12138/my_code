#ifndef __ACCEPT_H__
#define __ACCEPT_H__

#include "basic_head.h"
#include "reactor.h"

class Accept {
public:
    Accept(Reactor reactor/*...未定参数*/);
    virtual ~Accept(void);
private:
};

#endif