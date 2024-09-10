#include "../include/TaskQueue.h"
#include <clocale>

using std::queue;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::function;

TaskQueue::TaskQueue(size_t capacity)
: _capacity(capacity)
, _queue()
, _mutex()
, _notEmpty()
, _notFull()
, _flag(true)
{
    return;
}

/* 添加任务 */
void TaskQueue::push(Task && task){
    unique_lock<mutex> ul(_mutex);
    while(full()){
        _notFull.wait(ul);
    }
    _queue.push(std::move(task));
    _notEmpty.notify_one();
    return;
}

/* 获取任务 */
Task TaskQueue::pop(){
    unique_lock<mutex> ul(_mutex);
    while(empty() && _flag){
        _notEmpty.wait(ul);
    }

    if(_flag){
        Task task = _queue.front();
        _queue.pop();
        _notFull.notify_one();
        return task;
    }else{
        return nullptr;
    }
}

/* 判空 */
bool TaskQueue::empty() const{
    return _queue.size() == 0;
}

/* 判满 */
bool TaskQueue::full() const{
    return _queue.size() == _capacity;
}

/* 全部唤醒，有序退出 */
void TaskQueue::wakeup(){
    _flag = false;
    _notEmpty.notify_all();
    return;
}
