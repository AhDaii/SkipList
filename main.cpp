#include "include/SkipList.h"
#include "include/TaskQueue.h"

int main() {
    SkipList<int, std::string> s(10);
    s.insert(1, "Hello");
    s.insert(2, " ");
    s.insert(3, "World");
    s.insert(4, "!");
    s.del(2);
    s.print();
    s.dump_file();
    SkipList<int, std::string> t(10);
    t.load_file();
    t.print();
    t.search(1);
    t.search(5);
    return 0;
}
