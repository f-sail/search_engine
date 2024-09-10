#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"
#include <string>

class Acceptor{
public:
    Acceptor(const std::string& ip, unsigned short port);
    ~Acceptor();
    void ready();
    int accept();
    int fd() const;
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
public:
private:
    Socket _sock;
    InetAddress _addr;
};

#endif
