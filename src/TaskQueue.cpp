//
// Created by hunz1 on 2021/12/19.
//
#include "../include/TaskQueue.h"

Task::Task(): f(nullptr), arg(nullptr) {}

Task::Task(callback f, void* arg): f(f), arg(arg) {}
