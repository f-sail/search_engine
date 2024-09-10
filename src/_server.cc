#include "../include/log4cpp.h"

#include "../include/Acceptor.h"
#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"
#include "../include/TcpServer.h"
#include "../include/ThreadPool.h"

#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;



ThreadPool *gPool = nullptr;

class MyTask{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    {}
    void process(){
        //如何去处理_msg
        _msg;
        _con->sendInLoop(_msg);

    }
private:
    string _msg;
    TcpConnectionPtr _con;//要保证同一个连接
};

//连接建立的事件
void onNewConnection(const TcpConnectionPtr &con){
    cout << con->toString() << " has connected!!!" << endl;
}

//消息达到
void onMessage(const TcpConnectionPtr &con){
/* void onMessage(const TcpConnectionPtr &con, ThreadPool &pool) */
    puts("===============================new message================================");
    string msg = con->receive();
    cout << ">>recv msg from client: " << msg << endl;

    MyTask task(msg, con);
    gPool->addTask(std::bind(&MyTask::process, task));
}

//连接断开
void onClose(const TcpConnectionPtr &con){
    cout << con->toString() << " has closed!!!" << endl;
}

void test()
{
    ThreadPool pool(4, 10);
    pool.start();
    gPool = &pool;

    TcpServer server("127.0.0.1", 8888);
    server.setAllCallback(std::move(onNewConnection)
                          , std::move(onMessage)
                          , std::move(onClose));
    server.start();
}

int main(int args, char *argv[]){
    MyLogger *log = MyLogger::getInstance();
    LOG_INFO("The log is info message");
    LOG_ERROR("The log is error message");
    LOG_WARN("The log is warn message");
    LOG_DEBUG("The log is debug message");
    /* MyLogger::destroyInstance(); */
    test();

    return 0;
}

