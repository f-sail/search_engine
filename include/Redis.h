#ifndef __REDIS_H__
#define __REDIS_H__

#include <hiredis/hiredis.h>

class Redis{
public:
    static redisContext* getInstance();
    ~Redis();
private:
    Redis();
    Redis(const Redis&) = delete;
    Redis& operator=(const Redis&) = delete;
public:
private:
    static redisContext* _pInstance;
};


#endif
