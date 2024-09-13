#ifndef __TIMER_FD__
#define __TIMER_FD__

#include <functional>


class TimerFd{
public:
    using Task = std::function<void()>;
public:
    TimerFd(Task&& task, int initSec, int peridocSec);
    ~TimerFd();
    Task task();
    int fd();
private:
    int createTimerFd();
    void setTimerFd();
private:
    int _timerFd;
    int _initSec;
    int _peridocSec;
    Task _task;
};

#endif
