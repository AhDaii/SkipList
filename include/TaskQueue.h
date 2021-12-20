//
// Created by hunz1 on 2021/12/19.
//

#ifndef __SKIPLIST_TASKQUEUE_H__
#define __SKIPLIST_TASKQUEUE_H__

#include <mutex>
#include <queue>

using std::mutex;
using std::queue;
using callback = void (*)(void*);

class Task {
private:
    void* arg;
    callback f;

public:
    Task();
    Task(callback f, void* arg);
    void run();
};

class TaskQueue {
private:
    queue<Task> q;
    mutex m;

public:
    void add_task(Task t);
    void add_task(callback f, void* arg);
    Task get_task();
    int get_task_num();
};
#endif  // __SKIPLIST_TASKQUEUE_H__
