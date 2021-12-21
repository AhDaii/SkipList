#include "../include/ThreadPool.h"
#include <iostream>
using std::cout;
using std::endl;

ThreadPool::ThreadPool(int _min, int _max) : min_thread_num(_min), max_thread_num(_max) {
    busy_num = 0;
    exit_num = 0;
    alive_num = _min;
    is_close = false;
    task_queue = new TaskQueue();
    watcher_thread = new thread(&ThreadPool::watcher, this);
    worker_threads = new thread*[_max];
    for (int i = 0; i < _min; ++i)
        worker_threads[i] = new thread(&ThreadPool::worker, this);
}

void ThreadPool::add_task(Task t) {
    if (is_close == true)
        return;
    task_queue->add_task(t);
    empty_queue.notify_one();
}

void ThreadPool::worker() {
    while (1) {
        unique_lock<mutex> l(mutex_pool);
        while (task_queue->get_task_num() == 0 && is_close == false) {
            empty_queue.wait(l);
            if (exit_num > 0) {  // 当前线程是空闲线程
                --exit_num;
                if (alive_num > min_thread_num) {  // 如果存活线程还有，直接把当前线程释放掉
                    --alive_num;
                    l.unlock();
                    thread_exit();
                    return;
                }
            }
        }

        if (is_close == true) {
            l.unlock();
            thread_exit();
            return;
        }

        Task task = task_queue->get_task();
        l.unlock();
        cout << "thread \033[34m" << std::this_thread::get_id() << "\033[0m start working" << endl;

        // cout << "[now] " << busy_num << ' ' << alive_num << ' ' << exit_num << endl;
        ++busy_num;
        task.run();
        --busy_num;
        cout << "thread \033[34m" << std::this_thread::get_id() << "\033[0m end working" << endl;
    }
}

void ThreadPool::thread_exit() {
    --busy_num;
    for (int i = 0; i < max_thread_num; ++i)
        if (worker_threads[i] != nullptr && worker_threads[i]->get_id() == std::this_thread::get_id()) {
            cout << "threadExit() function: thread "
                 << std::this_thread::get_id() << " exiting..." << endl;
            worker_threads[i] = nullptr;
        }
}

void ThreadPool::watcher() {
    while (is_close == false) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        unique_lock<mutex> l(mutex_pool);
        int sz = task_queue->get_task_num();
        l.unlock();

        if (sz > alive_num && alive_num < max_thread_num) {  // 当前需要处理的任务比存活的线程还多，并且当前还可以开新的线程
            int cnt(0);
            {
                unique_lock<mutex> l_add(mutex_pool);
                for (int i = 0; i < max_thread_num && cnt < NUM && alive_num < max_thread_num; ++i)
                    if (worker_threads[i] == nullptr) {
                        ++cnt;
                        worker_threads[i] = new thread(&ThreadPool::worker, this);
                        ++alive_num;
                    }
            }
        }

        if (busy_num * 2 < alive_num && alive_num > min_thread_num) {  // 忙碌的线程很少，将一些空闲线程销毁
            exit_num = NUM;
            for (int i = 0; i < NUM; ++i)
                empty_queue.notify_one();
        }
    }
}

ThreadPool::~ThreadPool() {
    is_close = true;
    watcher_thread->join();
    empty_queue.notify_all();
    delete watcher_thread;
    delete[] worker_threads;
    delete task_queue;
}