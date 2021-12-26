#include "./include/ThreadPool.h"

#include <iostream>
#include <mutex>

void taskFunc(void* arg) {
    int num = *(int*)arg;
    std::cout << "thread \033[34m" << std::this_thread::get_id() << "\033[0m, num = " << num << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main() {
    ThreadPool* thread_pool = new ThreadPool(2, 10);
    int* num = nullptr;
    for (int i = 0; i < 101; ++i) {
        num = new int(i + 100);
        thread_pool->add_task(Task(taskFunc, num));
    }
    std::this_thread::sleep_for(std::chrono::seconds(15));
    delete num;
    return 0;
}