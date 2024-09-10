#ifndef __SOCKET_H__
#define __SOCKET_H__

class Socket{
public:
    ~Socket();
    Socket();
    Socket(const Socket&) = delete;
    Socket &operator=(const Socket&) = delete;
    explicit Socket(int fd);
    int fd() const;
    void shutDownWrite();
private:
    int _fd;
};

#endif
