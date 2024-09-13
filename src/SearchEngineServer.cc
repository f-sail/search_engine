#include "../include/SearchEngineServer.h"

#include "../include/log4cpp.h"
#include "../include/Acceptor.h"
#include "../include/Cache.h"
#include "../include/Configuration.h"
#include "../include/Dictionary.h"
#include "../include/EventLoop.h"
#include "../include/MessageHandler.h"
#include "../include/TcpConnection.h"
#include "../include/TcpServer.h"
#include "../include/ThreadPool.h"
#include "../include/TLV.h"
#include "../include/WebPageQuery.h"



#include <signal.h>
#include <unistd.h>

/* #include <cwchar> */
#include <iostream>

using std::cout;
using std::endl;


ThreadPool *gPool = nullptr;
static SearchEngineServer* gSES  = nullptr;


void startServer();
void stopServer();

SearchEngineServer::SearchEngineServer()
: _pool(std::stoi(getThreadNum()), std::stoi(getQueueSize()))
, _tcpServer(getIP(), static_cast<unsigned short>(stoi(getPort())))
{
    gPool = &_pool;
    gSES = this;
}

SearchEngineServer::~SearchEngineServer(){}

void SearchEngineServer::start(){
    signal(SIGINT, signalHandler);

    CacheManager::getInstance();
    Configuration::getInstance();
    Dictionary::getInstance();
    WebPageQuery::getInstance();

    _tcpServer.setAllCallback(std::move(CallBack::onNewConnection), std::move(CallBack::onMessage), std::move(CallBack::onClose));
    _pool.start();
    _tcpServer.start();
    return;
}

void SearchEngineServer::stop(){
    puts("stop()");
    _tcpServer.stop();
    _pool.stop();
    return;
}

void SearchEngineServer::signalHandler(int sig){
    return gSES->stop();
}

string SearchEngineServer::getThreadNum(){
    return Configuration::getInstance()->getConfigMap()["thread_num"];
}
string SearchEngineServer::getQueueSize(){
    return Configuration::getInstance()->getConfigMap()["queue_size"];
}
string SearchEngineServer::getIP(){
    return Configuration::getInstance()->getConfigMap()["ip"];
}
string SearchEngineServer::getPort(){
    return Configuration::getInstance()->getConfigMap()["port"];
}


namespace CallBack{
//连接建立的事件
void onNewConnection(const TcpConnectionPtr &con){
    cout << con->toString() << " has connected!!!" << endl;
}

//消息达到
void onMessage(const TcpConnectionPtr &con){
    TLV msg(con->receive());
    /* cout << ">>recv msg from client: " << msg.value << endl; */
    MessageHandler task(msg, con);
    gPool->addTask(std::bind(&MessageHandler::process, task));
}

//连接断开
void onClose(const TcpConnectionPtr &con){
    cout << con->toString() << " has closed!!!" << endl;
}

}// !namespace CallBack
