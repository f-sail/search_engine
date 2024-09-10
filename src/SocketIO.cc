#include "../include/log4cpp.h"
#include "../include/SocketIO.h"
/* #include "../include/TLV.h" */

#include <asm-generic/socket.h>
/* #include <stdio.h> */
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

SocketIO::SocketIO(int fd)
: _fd(fd)
{
    return;
}

SocketIO::~SocketIO(){}

int SocketIO::read(char *buf, size_t size){
    int count = 0;
    while(size > count){
        int ret = ::read(_fd, buf, size - count);
        if( -1 == ret){
            if(EINTR == errno){
                continue;
            }else{
                perror("read error -1");
                return -1;
            }
        }else if(0 == ret){
            break;
        }else{
            buf += ret;
            count += ret;
        }
    }
    return count;
}

Type SocketIO::readTLV(char *buf, size_t size){
    LOG_DEBUG("===========in readTLV==============");
    Type type;
    size_t length;

    int ret = readT(&type, sizeof(type));
    if(-1 == ret){
        return TYPE_ERROR;
    }else if(0 == ret){
        return TYPE_DISCONNECT;
    }

    ret = readL(&length, sizeof(length));
    if(-1 == ret){
        return TYPE_ERROR;
    }else if(0 == ret){
        return TYPE_DISCONNECT;
    }

    ret = readV(buf, length);
    if(-1 == ret){
        return TYPE_ERROR;
    }else if(0 == ret){
        return TYPE_DISCONNECT;
    }

    return type;
}

int SocketIO::write(const char* buf, size_t size){
    size_t count = 0;
    while(size > count){
        int ret = ::write(_fd, buf, size - count);
        if(-1 == ret){
            if(EINTR == errno){
                continue;
            }else{
                perror("write error -1");
                return -1;
            }
        }else if(0 == ret){
            break;
        }else{
            buf += ret;
            count += ret;
        }
    }
    return count;
}

int SocketIO::readT(Type* ptype, size_t size){
    LOG_DEBUG("===========in readT==============");
    while(1){
        int ret = recv(_fd, ptype, size, MSG_WAITALL);
        if(size == ret){
            return size;
        }else if(0 == ret){
            return 0;
        }else if(-1 == ret && EINTR == errno){
            continue;
        }else{
            perror("readT error -1");
            return -1;
        }
    }
}

int SocketIO::readL(size_t* plength, size_t size){
    LOG_DEBUG("===========in readL==============");
    while(1){
        int ret = recv(_fd, plength, size, MSG_WAITALL);
        if(size == ret){
            return size;
        }else if(0 == ret){
            return 0;
        }else if(-1 == ret && EINTR == errno){
            continue;
        }else{
            perror("readL error -1");
            return -1;
        }
    }
}
int SocketIO::readV(char* pvalue, size_t size){
    LOG_DEBUG("===========in readV==============");
    while(1){
        std::cout <<"size = " <<  size << std::endl;
        int ret = recv(_fd, pvalue, size, MSG_WAITALL);
        if(size == ret){
            return size;
        }else if(0 == ret){
            return 0;
        }else if(-1 == ret && EINTR == errno){
            LOG_INFO("EINTR == errno");
            continue;
        }else{
            perror("readV error -1");
            return -1;
        }
    }
}
