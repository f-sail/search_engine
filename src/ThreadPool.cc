#include "../include/ThreadPool.h"

#include <iostream>

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

void ThreadPool::addTask(Task &&task){
    if(task){
        _taskQueue.push(std::move(task));
    }
    return;
}

Task ThreadPool::getTask(){
    return _taskQueue.pop();
}

void ThreadPool::doTask(){
    //只要线程池不退出，就应该让工作线程一直执行任务
    //如果任务队列为空，应该让工作线程睡眠
    while(!_isExit){
        //获取任务，然后再执行任务
        Task taskcb = getTask();
        if(taskcb){
            /* ptask->process();//体现多态 */
            taskcb();//回调函数的执行
        }else{
            cout << "nullptr == taskcb" << endl;
        }
    }
    return;
}
