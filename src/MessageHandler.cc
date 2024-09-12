#include "../include/log4cpp.h"
#include "../include/EventLoop.h"
#include "../include/MessageHandler.h"
#include "../include/KeyRecommander.h"
#include "../include/WebPageSercher.h"

using std::string;

MessageHandler::MessageHandler(const TLV &msg, const TcpConnectionPtr &conn)
: _msg(msg)
, _conn(conn)
{}

void MessageHandler::process(){
    /* LOG_INFO("in process"); */
    switch(_msg.type){
        case TYPE_ERROR:
            _conn->sendInLoop(error());
            break;
        case TYPE_DISCONNECT:
            _conn->sendInLoop(disconnect());
            break;
        case TYPE_EMPTY:
            _conn->sendInLoop(emptymsg());
            break;
        case TYPE_RECOMMEND:
            _conn->sendInLoop(recommend());
            break;
        case TYPE_SEARCH:
            _conn->sendInLoop(search());
            break;
        default:
            _conn->sendInLoop(other());
            break;
    }// !switch
    return;
}


string MessageHandler::error(){
    return "ERROR";
}
string MessageHandler::disconnect(){
    return "todo";
}
string MessageHandler::emptymsg(){
    return ">> 请输入搜索内容...";
}
string MessageHandler::recommend(){
    return KeyRecommander(_msg.value).doQuery();
}
string MessageHandler::search(){
    return WebPageSercher(_msg.value).doQuery();
}
string MessageHandler::other(){
    return "todo";
}
