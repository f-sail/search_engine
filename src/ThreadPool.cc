#include "../include/ThreadPool.h"
#include "../include/log4cpp.h"
#include "../include/Cache.h"

#include <iostream>
#include <thread>

using std::cout;
using std::endl;
using std::thread;
using std::vector;
using std::function;

ThreadPool::ThreadPool(size_t thread_num, size_t queue_size)
: _threadNum(thread_num)
, _threads()
, _queSize(queue_size)
, _taskQueue(_queSize)
, _isExit(false)
{}

void ThreadPool::start(){
    /* 将子线程创建出来，并且存放在vector */
    for(size_t idx = 0; idx < _threadNum; ++idx){
        _threads.push_back(thread(&ThreadPool::doTask, this));
    }
    return;
}

void ThreadPool::stop(){
    /* 只要任务队列中还s务，就不应该让主线程急着向下执行去 */
    /* 回收子线程，在此处可以添加对应代码控制主线程向下走 */
    while(!_taskQueue.empty()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    /* 将标志位设置为true，表明线程池要退出 */
    _isExit = true;
    /* 可以在此处将所有等待在_notEmpty条件变量上的线程唤醒 */
    _taskQueue.wakeup();
    /* 让主线程等待子线程的退出 */
    for(auto &th : _threads){
        th.join();
    }
    return;
}

void ThreadPool::addTask(Task&& task){
    if(task){
        _taskQueue.push(std::move(task));
    }
    return;
}

Task ThreadPool::getTask(){
    return _taskQueue.pop();
}

void ThreadPool::doTask(){
    CacheManager::getInstance()->setCache(std::this_thread::get_id(), new Cache());
    while(!_isExit){
        Task taskcb = getTask();
        if(taskcb){
            taskcb();//回调函数的执行
        }else{
            cout << "nullptr == taskcb" << endl;
        }
    }
    return;
}
