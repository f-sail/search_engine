#include "../include/Acceptor.h"
#include "../include/log4cpp.h"

#include <stdio.h>

#include <string>

using std::string;

Acceptor::Acceptor(const string &ip, unsigned short port)
: _sock()
, _addr(ip, port)
{}

Acceptor::~Acceptor(){}

void Acceptor::ready(){
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr(){
    int on = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret){
        LOG_ERROR("setsockopt");
        /* perror("setsockopt"); */
        return;
    }
}

void Acceptor::setReusePort(){
    int on = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if(-1 == ret){
        LOG_ERROR("setsockopt");
        /* perror("setsockopt"); */
        return;
    }
}

void Acceptor::bind(){
    int ret = ::bind(_sock.fd(), 
                     (struct sockaddr *)_addr.getInetAddrPtr(),
                     sizeof(struct sockaddr));
    if(-1 == ret){
        LOG_ERROR("bind");
        /* perror("bind"); */
        return;
    }
}

void Acceptor::listen(){
    int ret = ::listen(_sock.fd(), 128);
    if(-1 == ret){
        LOG_ERROR("listen");
        /* perror("listen"); */
        return;
    }
}

int Acceptor::accept(){
    int connfd = ::accept(_sock.fd(), nullptr, nullptr);
    if(-1 == connfd){
        LOG_ERROR("accept");
        /* perror("accept"); */
        return -1;
    }
    return connfd;
}

int Acceptor::fd() const{
    return _sock.fd();
}
