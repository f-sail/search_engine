#include "../include/TimerFd.h"
#include "../include/log4cpp.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <memory.h>

#include <iostream>
using std::cout;
using std::endl;


TimerFd::TimerFd(Task&& task, int initSec, int peridocSec)
: _timerFd(createTimerFd())
, _initSec(initSec)
, _peridocSec(peridocSec)
, _task(std::move(task))
{
    setTimerFd();
    return;
}

TimerFd::~TimerFd(){
    close(_timerFd);
    return;
}

TimerFd::Task TimerFd::task(){
    return _task;
}

int TimerFd::fd(){
    return _timerFd;
}

int TimerFd::createTimerFd(){
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0){
        LOG_ERROR("timerfd_create failed");
        return -1;
    }
    return fd;
}

void TimerFd::setTimerFd(){
    struct itimerspec newValue;
    ::memset(&newValue, 0, sizeof(newValue));

    newValue.it_value.tv_sec = _initSec;
    newValue.it_value.tv_nsec = 0;

    newValue.it_interval.tv_sec = _peridocSec;
    newValue.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timerFd, 0, &newValue, nullptr);
    if(ret < 0){
        LOG_ERROR("timerfd_settime failed");
        return;
    }
}

