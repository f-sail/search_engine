#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"
#include "../include/log4cpp.h"
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <sys/socket.h>

#define BUFF_SIZE 65535

using std::string;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _loop(loop)
, _sockIO(fd)
, _sock(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
{}

void TcpConnection::send(const string& msg){
    /* std::cout << " msg = " << msg <<"\n"; */
    size_t ret = _sockIO.write(msg.c_str(), msg.size());
    if(ret != msg.size()){
        LOG_WARN(" write count != msg.size()");
    }
    return;
}

void TcpConnection::sendInLoop(const string& msg){
    if(_loop){
        _loop->runInLoop(bind(&TcpConnection::send, this, msg));
    }
    return;
}

TLV TcpConnection::receive(){
    TLV msg(_sockIO.readTLV());
    return msg;
}

bool TcpConnection::isClosed() const{
    char flag;
    int ret = ::recv(_sock.fd(), &flag, sizeof(flag), MSG_PEEK);
    return 0 == ret;
}

string TcpConnection::toString(){
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();
    return oss.str();
}

/* 获取本端的网络地址信息 */
InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    //获取本端地址的函数getsockname
    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret){
        perror("getpeername");
    }
    return InetAddress(addr);
}

/* 获取对端的网络地址信息 */
InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    //获取对端地址的函数getpeername
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret){
        perror("getpeername");
    }

    return InetAddress(addr);
}

void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback &cb){
    _onNewConnection = cb;
    return;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb){
    _onMessage = cb;
    return;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb){
    _onClose = cb;
    return;
}


void TcpConnection::handleNewConnectionCallback(){
    if(_onNewConnection){
        _onNewConnection(shared_from_this());
    }else{
        std::cerr << "_onNewConnection == nullptr\n";
    }
    return;
}

void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }else{
        std::cerr << "_onMessage == nullptr\n";
    }
    return;
}

void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }else{
        std::cerr << "_onClose == nullptr\n";
    }
    return;
}
