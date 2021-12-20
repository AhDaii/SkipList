#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <thread>

using std::atomic_bool;
using std::atomic_int;
using std::condition_variable;
using std::mutex;
using std::thread;
using std::unique_lock;

class ThreadPool {
private:
    TaskQueue* task_queue;                     // 任务队列
    thread *watcher_thread, **worker_threads;  // 监视者线程和工作者线程
    atomic_int busy_num;                       // 忙碌线程数
    atomic_int exit_num;                       // 即将要销毁的线程数
    atomic_int alive_num;                      // 存活的线程数
    int min_thread_num, max_thread_num;        // 线程最大最小数
    mutex mutex_pool;                          // 线程池互斥量
    condition_variable empty_queue;            // 空队列的条件变量
    atomic_bool is_close;                      // 线程池是否关闭

    enum { NUM = 2 };

public:
    ThreadPool(int _min, int _max);
    void add_task(Task t);
    void worker();
    void watcher();
    void thread_exit();
    ~ThreadPool();
};

#endif  // __THREADPOOL_H__