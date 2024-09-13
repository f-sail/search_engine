#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "../include/Cache.h"
#include "../include/TaskQueue.h"

#include <thread>
#include <vector>
#include <functional>

class ThreadPool{
public:
    ThreadPool(size_t thread_num, size_t queue_size);
    ~ThreadPool() = default;
    
    /* 线程池的启动与停止 */
    void start();
    void stop();
    /* 任务的添加与获取 */
    void addTask(Task &&task);
private:
    Task getTask();
    /* 线程池交给工作线程执行的任务（线程入口函数） */
    void doTask();
private:
    size_t _threadNum;                  //子线程的数目
    std::vector<std::thread> _threads;  //存放线程的容器
    size_t _queSize;                    //任务队列的大小
    TaskQueue _taskQueue;               //任务队列
    bool _isExit;                       //标识线程池是否退出的标志位
};

#endif
