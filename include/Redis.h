#ifndef __REDIS_H__
#define __REDIS_H__

#include <hiredis/hiredis.h>
#include <hiredis/read.h>
#include <string>

class Redis{
public:
    static Redis* getInstance();
    void setKV(const std::string& key, const std::string& value);
    std::string getValue(const std::string& key);
    redisContext* ptr();
    ~Redis();
private:
    Redis();
    Redis(const Redis&) = delete;
    Redis& operator=(const Redis&) = delete;
public:
private:
    static Redis* _pInstance;
    redisContext* _readisPtr;
};


#endif
