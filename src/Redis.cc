#include "../include/log4cpp.h"
#include "../include/Redis.h"
#include <hiredis/hiredis.h>
#include <hiredis/read.h>

Redis* Redis::_pInstance = nullptr;

Redis* Redis::getInstance(){
    puts("in getInstance");
    if(!_pInstance){
        _pInstance = new Redis();
    }
    puts("out getInstance");
    return _pInstance;
}

redisContext* Redis::ptr(){
    if(!_readisPtr){
        _readisPtr = redisConnect("127.0.0.1", 6379);
        if(nullptr == _readisPtr || _readisPtr->err){
            LOG_ERROR("redis connection failed!");
        }else{
            puts(">> redis ready!");
        }
    }
    return _readisPtr;
}

void Redis::setKV(const std::string& key, const std::string& value){
    redisReply* reply = (redisReply*)redisCommand(ptr(), "SET %s %s", key.c_str(), value.c_str());
    if(nullptr == reply){
        LOG_ERROR(ptr()->errstr);
        redisFree(_readisPtr);
        _readisPtr = nullptr;
        return;
    }
    if(REDIS_REPLY_ERROR == reply->type){
        LOG_ERROR(reply->str);
    }else{
        LOG_INFO("key set successfully");
    }
    return;
}

std::string Redis::getValue(const std::string& key){
    redisReply* reply = (redisReply*)redisCommand(ptr(), "GET %s", key.c_str());
    if(nullptr == reply){
        LOG_ERROR(ptr()->errstr);
        redisFree(_readisPtr);
        _readisPtr = nullptr;
        static string nullstr = string();
        return nullstr;
    }
    if(REDIS_REPLY_ERROR == reply->type){
        LOG_ERROR(reply->str);
        return nullptr;
    }else{
        LOG_INFO("key set successfully");
        return string(reply->str);
    }
}

Redis::Redis()
: _readisPtr(nullptr)
{
    ptr();
}

Redis::~Redis(){
    if(_pInstance){
        redisFree(_readisPtr);
        _readisPtr = nullptr;

        delete _pInstance;
        _pInstance = nullptr;
    }
    return;
}
