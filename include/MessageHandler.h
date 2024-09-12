#ifndef __MESSAGE_HANDLER_H__
#define __MESSAGE_HANDLER_H__

#include "TLV.h"
#include "TcpConnection.h"
#include "EventLoop.h"

class MessageHandler{
public:
    MessageHandler(const TLV &msg, const TcpConnectionPtr &con);
    void process();
private:
    std::string error();
    std::string disconnect();
    std::string emptymsg();
    std::string recommend();
    std::string search();
    std::string other();
private:
    TLV _msg;
    TcpConnectionPtr _conn;//要保证同一个连接
};

#endif
