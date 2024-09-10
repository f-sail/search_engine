#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <type_traits>

using Task = std::function<void()>;

class TaskQueue{
public:
    TaskQueue(size_t capacity);
    ~TaskQueue() = default;

    void push(Task && task);
    Task pop();

    bool empty() const;
    bool full() const;
    
    void wakeup();
private:
    size_t _capacity;
    std::queue<Task> _queue;
    std::mutex _mutex;
    std::condition_variable _notEmpty;
    std::condition_variable _notFull;
    bool _flag;
};

#endif
