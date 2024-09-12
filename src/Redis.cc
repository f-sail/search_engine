#include "../include/log4cpp.h"
#include "../include/Redis.h"
#include <hiredis/hiredis.h>
#include <hiredis/read.h>

redisContext* Redis::_pInstance = nullptr;

redisContext* Redis::getInstance(){
    if(!_pInstance){
        _pInstance = redisConnect("127.0.0.1", 6379);
    }
    if(_pInstance){
        puts(">> redis ready!");
    }else{
        LOG_ERROR("redis connection failed!");
    }
    return _pInstance;
}

Redis::Redis(){}

Redis::~Redis(){
    if(_pInstance){
        redisFree(_pInstance);
        _pInstance = nullptr;
    }
    return;
}
