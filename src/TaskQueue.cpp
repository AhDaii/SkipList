//
// Created by hunz1 on 2021/12/19.
//
#include "../include/TaskQueue.h"

using std::unique_lock;

Task::Task() : f(nullptr), arg(nullptr) {}

Task::Task(callback f, void* arg) : f(f), arg(arg) {}

void Task::run() {
    f(arg);
}

void TaskQueue::add_task(Task t) {
    unique_lock<mutex> l(m);
    q.push(t);
}

void TaskQueue::add_task(callback f, void* arg) {
    unique_lock<mutex> l(m);
    q.push({f, arg});
}

Task TaskQueue::get_task() {
    unique_lock<mutex> l(m);
    Task t;
    if (!q.empty()) {
        t = q.front();
        q.pop();
    }
    return t;
}

int TaskQueue::get_task_num() {
    unique_lock<mutex> l(m);
    return q.size();
}
