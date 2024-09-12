#include "../include/_server.h"

#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;

ThreadPool *gPool = nullptr;

namespace CallBack{
/* 连接建立的事件 */
void onNewConnection(const TcpConnectionPtr &con);
/* 消息达到 */
void onMessage(const TcpConnectionPtr &con);
/* 连接断开 */
void onClose(const TcpConnectionPtr &con);
}

void startServer()
{
    ThreadPool pool(4, 10);
    pool.start();
    gPool = &pool;
    string ip = Configuration::getInstance()->getConfigMap()["ip"];
    string port = Configuration::getInstance()->getConfigMap()["port"];
    TcpServer server(ip, static_cast<unsigned short>(atoi(port.c_str())));
    server.setAllCallback(std::move(CallBack::onNewConnection), std::move(CallBack::onMessage), std::move(CallBack::onClose));
    server.start();
}

int main(int args, char *argv[]){
    Configuration::getInstance()->chdir();

puts("----");
    Dictionary::getInstance();
puts("----");
    WebPages::getInstance();
puts("----");

    startServer();

    return 0;
}

namespace CallBack{
//连接建立的事件
void onNewConnection(const TcpConnectionPtr &con){
    cout << con->toString() << " has connected!!!" << endl;
}

//消息达到
void onMessage(const TcpConnectionPtr &con){
/* void onMessage(const TcpConnectionPtr &con, ThreadPool &pool) */
    TLV msg(con->receive());
    cout << ">>recv msg from client: " << msg.value << endl;
    MessageHandler task(msg, con);
    gPool->addTask(std::bind(&MessageHandler::process, task));
}

//连接断开
void onClose(const TcpConnectionPtr &con){
    cout << con->toString() << " has closed!!!" << endl;
}

}// !namespace CallBack
