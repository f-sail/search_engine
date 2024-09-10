#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

#include "../include/TLV.h"
#include <cstddef>
#include <stdio.h>

class SocketIO{
public:
    ~SocketIO();
    explicit SocketIO(int fd);
    int read(char *buf, size_t size);
    Type readTLV(char *buf, size_t size);
    int write(const char *buf, size_t size);
private:
    int readT(Type* ptype, size_t size);
    int readL(size_t* plength, size_t size);
    int readV(char* pvalue, size_t size);
private:
    int _fd;
};

#endif
