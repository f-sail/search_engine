#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <mutex>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;
using std::mutex;
using std::lock_guard;

class Acceptor;//前向声明
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;

class EventLoop{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    //循环是否在进行
    void loop();
    void unloop();

    //封装了epoll_wait函数
    void waitEpollFd();

    //处理新的连接请求
    void handleNewConnection();

    //老的连接上数据发过来
    void handleMessage(int fd);

    //创建文件epoll的文件描述符
    int createEpollFd();

    //将文件描述符放在红黑树上进行监听
    void addEpollReadFd(int fd);
    
    //将文件描述符从红黑树上取消监听
    void delEpollReadFd(int fd);

public:
    //注册三个事件（起到桥梁作用，目的是为了传递给TcpConnection）
    void setNewConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

public:
    //创建用于通知的文件描述符
    int createEventFd();
    //封装read
    void handleRead();
    //封装write
    void wakeup();
    void doPengdingFunctors();
    void runInLoop(Functor &&cb);

private:
    Acceptor &_acceptor;                //Acceptor的引用
    mutex _mutex;                       //互斥访问vector
    
    int _epfd;                          //epoll_create创建的文件描述符
    int _evtfd;                         //用于通知的文件描述符
                                        //
    bool _isLooping;                    //标识循环是否进行的标志
                                        
    vector<Functor> _pengdings;         //存放要执行的"任务"
    map<int, TcpConnectionPtr> _conns;  //存放文件描述符与TcpConnection连接的键值对
    vector<struct epoll_event> _evtList;//存放就绪文件描述符的结构体数组

    TcpConnectionCallback _onNewConnection; //连接建立
    TcpConnectionCallback _onMessage;       //消息到达
    TcpConnectionCallback _onClose;         //连接断开


};

#endif
