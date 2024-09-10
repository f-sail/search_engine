#include "../include/EventLoop.h"
#include "../include/Acceptor.h"
#include "../include/TcpConnection.h"
#include "../include/log4cpp.h"

#include <string>
#include <unistd.h>
#include <sys/eventfd.h>

#include <fstream>
#include <sstream>
#include <iostream>


using std::cerr;
using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollFd())
, _evtList(1024)//采用个count个value进行初始化
, _isLooping(false)
, _acceptor(acceptor)
, _evtfd(createEventFd())//创建用于通知的文件描述符
, _pengdings()
, _mutex()
{
    //需要获取listenfd，然后放在红黑树上进行监听
    int listenfd = _acceptor.fd();
    addEpollReadFd(listenfd);

    //监听用于通信的文件描述符
    addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop(){
    close(_epfd);
    close(_evtfd);
}

//循环是否在进行
void EventLoop::loop(){
    _isLooping = true;
    while(_isLooping){
        waitEpollFd();
    }
}

void EventLoop::unloop(){
    _isLooping = false;
}

//封装了epoll_wait函数
void EventLoop::waitEpollFd(){
    int nready = 0;
    do{
        //vector中如何获取第一个元素的首地址
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 3000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready){
        LOG_ERROR("-1 == epoll_wait()");
        return;
    }
    else if(0 == nready){
        LOG_INFO("epoll_wait timeout!!!")
    }else{
        //需要考虑vector，也就是_evtList的扩容问题(1024)
        if((int)_evtList.size() == nready){
            _evtList.reserve(2 * nready);
        }

        for(int idx = 0; idx < nready; ++idx){
            int listenfd = _acceptor.fd();
            int fd = _evtList[idx].data.fd;

            if(fd == listenfd){         //有新的连接请求
                handleNewConnection();
            }else if(fd == _evtfd){     //通信的文件描述符就绪
                handleRead();
                //可以执行所有的"任务",就是遍历vector
                doPengdingFunctors();
                /* for(auto &cb : _pengdings) */
                /* { */
                /*     cb(); */
                /* } */
            }else{
                handleMessage(fd);//处理老的连接
            }
        }// !for()
    }
}

//处理新的连接请求
void EventLoop::handleNewConnection(){
    //只要accept函数有正确返回结果，就表明三次握手建立成功
    int connfd =  _acceptor.accept();
    if(connfd < 0){
        LOG_WARN("handleNewConnection failed");
        /* perror("handleNewConnection"); */
        return;
    }

    //将connfd放在红黑树上进行监听
    addEpollReadFd(connfd);

    //可以使用文件描述符connfd创建TcpConnection连接的对象
    TcpConnectionPtr con(new TcpConnection(connfd, this));
    
    //将三个事件转接个TcpConnection的对象
    con->setNewConnectionCallback(_onNewConnection); //注册连接建立的事件
    con->setMessageCallback(_onMessage);             //注册消息到达的事件
    con->setCloseCallback(_onClose);                 //注册连接断开的事件

    /* _conns.insert({connfd, con}); */
    _conns[connfd] = con;//存放键值对
                         
    //连接建立的事件已经满足
    con->handleNewConnectionCallback();//执行连接建立的事件
}

//老的连接上数据发过来
void EventLoop::handleMessage(int fd){
    auto it = _conns.find(fd);
    if(it != _conns.end()){
        //isClose函数的作用就是看有没有断开
        bool flag = it->second->isClosed();
        if(flag){
            //连接已经断开了
            it->second->handleCloseCallback();//连接断开的事件
            delEpollReadFd(fd);//取消监听（从红黑树上删除）
            _conns.erase(it);//删除文件描述符与连接的键值对
        }else{
            it->second->handleMessageCallback();//消息达到的事件
        }
    }else{
        LOG_WARN("连接不存在");
    }
    return;
}

//创建文件epoll的文件描述符
int EventLoop::createEpollFd(){
    int fd = epoll_create(10);
    if(fd < 0){
        LOG_ERROR("createEpollFd failed");
        perror("createEpollFd");
        return -1;
    }
    return fd;
}

//将文件描述符放在红黑树上进行监听
void EventLoop::addEpollReadFd(int fd){
    struct epoll_event evt;
    evt.data.fd = fd;//要监听的文件描述符
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret < 0){
        LOG_ERROR("addEpollFd failed");
        perror("addEpollReadFd");
        return;
    }
}

//将文件描述符从红黑树上取消监听
void EventLoop::delEpollReadFd(int fd){
    struct epoll_event evt;
    evt.data.fd = fd;//要监听的文件描述符
    evt.events = EPOLLIN;

    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0){
        LOG_ERROR("delEpollFd failed");
        perror("delEpollReadFd");
        return;
    }

}
//注册三个事件（起到桥梁作用，目的是为了传递给TcpConnection）
void EventLoop::setNewConnectionCallback(TcpConnectionCallback &&cb){
    _onNewConnection = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb){
    _onMessage = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb){
    _onClose = std::move(cb);
}
//创建用于通知的文件描述符
int EventLoop::createEventFd(){
    int fd = eventfd(0, 0);
    if(fd < 0){
        perror("createEventFd");
        return -1;
    }

    return fd;
}

//封装read
void EventLoop::handleRead(){
    uint64_t one = 1;
    ssize_t ret = read(_evtfd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t)){
        perror("handleRead");
    }
}
//封装write
void EventLoop::wakeup(){
    uint64_t one = 1;
    ssize_t ret = write(_evtfd, &one, sizeof(uint64_t));
    if(ret != sizeof(uint64_t)){
        perror("wakeup");
    }
}

void EventLoop::doPengdingFunctors(){
    vector<Functor> tmp;
    {
        lock_guard<mutex> lg(_mutex);
        tmp.swap(_pengdings);
    }

    //执行所有的任务:此处的“任务”就是线程池处理好之后的数据msg
    //以及发送数据的函数send，以及执行send函数的TcpConnection的
    //对象this,
    for(auto &cb : tmp){
        cb();
    }
}

void EventLoop::runInLoop(Functor &&cb){
    {
        lock_guard<mutex> lg(_mutex);
        _pengdings.push_back(std::move(cb));
    }

    wakeup();
}
