#ifndef __TLV_H__
#define __TLV_H__

#include <stdio.h>
#include <string>

enum Type{
    TYPE_ERROR = -1,    // 错误
    TYPE_DISCONNECT,    // 
    TYPE_EMPTY,         // 空value
    TYPE_RECOMMEND,     // 推荐
    TYPE_SEARCH,        // 搜索
};

struct TLV{
    Type type;
    size_t len = 0;
    std::string value;
};

#endif
