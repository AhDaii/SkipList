//
// Created by hunz1 on 2021/9/24.
//
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "SkipList.h"

#define MAXN 1000000

int main() {
    srand(time(NULL));
    auto start = std::chrono::high_resolution_clock::now();
    SkipList<int, int> skipList(18);
    for(int i = 0; i < MAXN; ++ i) {
        skipList.insert(rand(), rand());
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> use = end - start;
    std::cout << "Insert " << MAXN << " data use " << use.count() << "s" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < MAXN; ++ i) {
        skipList.search(rand());
    }
    end = std::chrono::high_resolution_clock::now();
    use = end - start;
    std::cout << "Search " << MAXN << " data use " << use.count() << "s" << std::endl;
    return 0;

}