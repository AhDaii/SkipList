//
// Created by hunz1 on 2021/12/19.
//

#ifndef SKIPLIST_TASKQUEUE_H
#define SKIPLIST_TASKQUEUE_H

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
};

#endif  // SKIPLIST_TASKQUEUE_H
