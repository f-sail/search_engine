#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
    InetAddress(const std::string &ip, unsigned short port);
    InetAddress(const struct sockaddr_in &addr);
    ~InetAddress();
    std::string ip() const;
    unsigned short port() const;
    const struct sockaddr_in *getInetAddrPtr() const;
private:
    struct sockaddr_in _addr;
};

#endif
