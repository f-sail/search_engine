#include "../include/log4cpp.h"
#include "../include/SocketIO.h"
#include "../include/TLV.h"

#include <asm-generic/socket.h>
/* #include <stdio.h> */
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

using std::cout;

#define BUFF_SIZE 65535

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

TLV SocketIO::readTLV(){
    /* LOG_DEBUG("===========in readTLV=============="); */
    TLV msg;

    int ret = readT(&msg.type, sizeof(msg.type));
    if(-1 == ret){
        msg.type = TYPE_ERROR;
        return msg;
    }else if(0 == ret){
        msg.type = TYPE_DISCONNECT;
        return msg;
    }

    ret = readL(&msg.len, sizeof(msg.len));
    if(-1 == ret){
        msg.type = TYPE_ERROR;
        return msg;
    }else if(0 == ret){
        msg.type = TYPE_DISCONNECT;
        return msg;
    }
    
    if(0 == msg.len){
        msg.type = TYPE_EMPTY;
        return msg;
    }

    ret = readV(msg.value, msg.len);
    if(-1 == ret){
        msg.type = TYPE_ERROR;
        return msg;
    }else if(0 == ret){
        msg.type = TYPE_DISCONNECT;
        return msg;
    }

    return msg;
}

int SocketIO::write(const char* buf, size_t size){
    size_t count = 0;
    while(size > count){
        int ret = ::send(_fd, buf, size - count, MSG_NOSIGNAL);
        if(-1 == ret){
            if(EINTR == errno){
                continue;
            }else{
                LOG_ERROR("write error -1");
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
    /* LOG_DEBUG("===========in readT=============="); */
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

int SocketIO::readL(size_t* plen, size_t size){
    /* LOG_DEBUG("===========in readL=============="); */
    while(1){
        int ret = recv(_fd, plen, size, MSG_WAITALL);
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
int SocketIO::readV(string &value, size_t size){
    /* LOG_DEBUG("===========in readV=============="); */
    std::cout << "size = " << size << "\n";
    char buff[BUFF_SIZE] = {0};
    while(1){
        int ret = ::recv(_fd, buff, size, MSG_WAITALL);
        if(size == ret){
            value = std::move(buff);
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
