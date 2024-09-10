#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

#include <functional>
#include <memory>

/* using std::function; */
/* using std::shared_ptr; */

class EventLoop;//前向声明

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
public:
    explicit TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection() = default;
    void send(const std::string &msg);
    void sendInLoop(const std::string &msg);
    std::string receive();
    //判断连接是否断开（被动断开）
    bool isClosed() const;

    //为了方便调试的函数
    std::string toString();

private:
    //获取本端地址与对端地址
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

public:
    //注册三个回调
    void setNewConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);
    
    //执行三个回调
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    EventLoop *_loop;   //要知道EventLoop的存在才能将数据发给EventLoop
    SocketIO _sockIO;

    //为了调试而加入的三个数据成员
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onNewConnection; //连接建立
    TcpConnectionCallback _onMessage;       //消息到达
    TcpConnectionCallback _onClose;         //连接断开
};

#endif
