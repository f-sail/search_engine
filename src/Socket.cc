#include "../include/Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

Socket::Socket(){
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0){
        perror("socket");
    }
    return;
}

Socket::Socket(int fd)
: _fd(fd)
{
    return;
}

Socket::~Socket(){
    if(_fd >= 0){
        close(_fd);
        _fd = -1;
    }
    return;
}

int Socket::fd() const{
    return _fd;
}

void Socket::shutDownWrite(){
    int ret = shutdown(_fd, SHUT_WR);
    if(ret){
        perror("shutdown");
    }
    return;
}
