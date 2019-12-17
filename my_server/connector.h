#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "basic_head.h"
#include "reactor.h"

class Connector {
public:
    Connector(Reactor reactor/*...未定参数*/);
    virtual ~Connector(void);
private:
};

#endif