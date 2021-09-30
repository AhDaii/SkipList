//
// Created by hunz1 on 2021/9/24.
//
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "SkipList.h"

#define MAXN 1000

inline int get_num() {
    return rand() %  MAXN;
}

int main() {
    srand(time(NULL));
    SkipList<int, int> skipList(18);
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < MAXN; ++ i) {
        skipList.insert(get_num(), get_num());
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> use = end - start;
    std::cout << "Insert " << MAXN << " data use " << use.count() << "s" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < MAXN; ++ i) {
        skipList.search(get_num());
    }
    end = std::chrono::high_resolution_clock::now();
    use = end - start;
    std::cout << "Search " << MAXN << " data use " << use.count() << "s" << std::endl;

    skipList.print();
    return 0;

}