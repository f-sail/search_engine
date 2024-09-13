#ifndef __SEARCH_ENGINE_SERVER_H__
#define __SEARCH_ENGINE_SERVER_H__

#include "TcpServer.h"
#include "ThreadPool.h"

#include <string>


namespace CallBack{
/* 连接建立的事件 */
void onNewConnection(const TcpConnectionPtr &con);
/* 消息达到 */
void onMessage(const TcpConnectionPtr &con);
/* 连接断开 */
void onClose(const TcpConnectionPtr &con);
}// !namespace CallBack


class SearchEngineServer{
public:
    SearchEngineServer();
    ~SearchEngineServer();
    void start();
    void stop();
private:
    static void signalHandler(int sig);
    std::string getThreadNum();
    std::string getQueueSize();
    std::string getIP();
    std::string getPort();
    SearchEngineServer(const SearchEngineServer&) = delete;
    SearchEngineServer& operator=(const SearchEngineServer&) = delete;
private:
    ThreadPool _pool;
    TcpServer _tcpServer;
};







#endif
