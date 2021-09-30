//
// Created by hunz1 on 2021/9/23.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include <iostream>
#include <ctime>
#include <climits>
#include <fstream>
#include <typeinfo>
#include <string>
#include <boost/lexical_cast.hpp>

template <typename K, typename V>
class Node {
private:
    K key;
    V value;
public:
    Node<K, V> **level_next; // 存储当前节点在每一层的下一个节点地址。
    int level;

    Node();
    Node(K k, V v, int l);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V v);
};

template <typename K, typename V>
class SkipList {
private:
    Node<K, V>* head;
    int max_level, cur_level;
    int random_level() const;
    static const char separator = ':';
public:
    SkipList();
    explicit SkipList(int m);
    ~SkipList();
    void print() const;
    bool insert(K k, V v);
    bool search(K k) const;
    bool del(K k);
    bool dump_file(char* filename = "./db.txt") const;
    bool load_file(char* filename = "./db.txt");
};

template <typename K, typename V>
Node<K,V>::Node(): key(K()), value(V()), level(0) {
    level_next = nullptr;
}

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, const int l): key(k), value(v), level(l) {
    level_next = new Node<K, V>*[level + 1]; // 申请level + 1个(从0到level)以Node*为元素的数组
    for(int i = 0; i < l; ++i)
        level_next[i] = nullptr;
}

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
}

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
}

template<typename K, typename V>
Node<K, V>::~Node() {
    delete [] level_next;
}

template<typename K, typename V>
void Node<K, V>::set_value(V v) {
    value = v;
}

template<typename K, typename V>
SkipList<K, V>::SkipList(): max_level(0), cur_level(0) {
    head = nullptr;
    srand(time(NULL));
}

template<typename K, typename V>
SkipList<K, V>::SkipList(int m): max_level(m), cur_level(0) {
    head = new Node<K, V>(K(), V(), max_level); // 给head申请max_level层，来使head可以访问到所有层
    srand(time(NULL));
}

template<typename K, typename V>
void SkipList<K, V>::print() const{
    if(head->level_next[0] == nullptr) { // 第0层就没数据，表明整个跳表是空的
        std::cout << "Skip list is empty!" << std::endl;
        return ;
    }
    std::cout << "===============SKIPLIST DATA===============" << std::endl;
    for(int i = cur_level; i >= 0; -- i) {
        std::cout << "LEVEL " << i << " - ";
        Node<K, V>* cur = head->level_next[i];
        while(cur != nullptr) {
            std::cout << '(' << cur->get_key() << ':' << cur->get_value() << ')' << ',';
            cur = cur->level_next[i];
        }
        std::cout << "\b\n";
    }
    std::cout << "====================END====================" << std::endl;
}

template<typename K, typename V>
bool SkipList<K, V>::insert(K k, V v) {

    // 1. 记录每一层小于k值的最大值的结点
    Node<K, V>** record = new Node<K, V>*[max_level + 1];
    Node<K, V>* cur = head;
    for(int i = cur_level; i >= 0; -- i) {
        while(cur->level_next[i] != nullptr && cur->level_next[i]->get_key() < k) {
            cur = cur->level_next[i];
        }
        record[i] = cur;
    }
    // 2. 转到最底层准备进行插入操作
    cur = cur->level_next[0];

    // 3. 判断跳表中是否已有该数据
    if(cur != nullptr && cur->get_key() == k) {
        cur->set_value(v);
        std::cout << "[INSERT CHANGE] The key " << k << " is changed!" << std::endl;
        return false;
    }

    // 4.如果没有该数据，执行插入操作
    int r_level = random_level();

    if(r_level > cur_level) { // 如果生成的随机层数比当前的要高, 就需要在record的更高层添加head，表示这一层将会有新的数据
        for(int i = cur_level + 1; i <= r_level; ++i)
            record[i] = head;
        cur_level = r_level;
    }

    Node<K, V>* insert_node = new Node<K, V>(k, v, r_level);
    for(int i = 0; i <= r_level; ++i) {
        insert_node->level_next[i] = record[i]->level_next[i];
        record[i]->level_next[i] = insert_node;
    }
    std::cout << "[INFO] Insert (" << k << ", " << v << ") succeeded!" << std::endl;
    delete [] record;
    return true;
}

/**
 * 跳表中为了防止插入的数据大量堆积在两个索引之间，需要让一些数据提升层数。
 * 通常的做法是使用随机函数来让数据随机提升层数，防止跳表的退化，提升效率。
 * 新插入的数据在第0层有50%的概率上升到上一层，上升后仍有50%的概率上升到上一层，
 * 可以通过生成随机数，判断奇偶来实现，因为一个随机数为奇偶的概率都为50%
 */
template<typename K, typename V>
int SkipList<K, V>::random_level() const{
    int level = 0;
    while(rand() & 1) {
        ++ level;
    }
    return level > max_level ? max_level : level;               // 防止生成的层数过高
}

template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    for(Node<K, V>* cur = head->level_next[0], *tmp; cur != nullptr; cur = tmp) {
        tmp = cur->level_next[0];
        delete cur;
    }
    delete head;
}

template<typename K, typename V>
bool SkipList<K, V>::search(K k) const{
    if(head->level_next[0] == nullptr) {
        std::cout << "[INFO] Key " << k << " is not exist!" << std::endl;
        return false;
    }
    Node<K, V>* cur = head;
    for(int i = cur_level; i >= 0; -- i) {
        while(cur->level_next[i] != nullptr && cur->level_next[i]->get_key() <= k)
            cur = cur->level_next[i];
        if(cur->get_key() == k) { // 在高层查找到就直接退出，不必往下继续查找
            std::cout << "[INFO] Key " << k << " is exist!" << std::endl;
            return true;
        }
    }
    if(cur == nullptr || cur->get_key() != k) {
        std::cout << "[INFO] Key " << k << " is not exist!" << std::endl;
        return false;
    }
    std::cout << "[INFO] Key " << k << " is exist!" << std::endl;
    return true;
}

template<typename K, typename V>
bool SkipList<K, V>::del(K k) {
    bool find = false;
    Node<K, V> *cur = head, *tmp, **test;
    int tmp_level = 0;

    for(int i = cur_level; i >= 0; -- i) { // 首先检测跳表中是否有这个数据
        while(cur->level_next[i] != nullptr && cur->level_next[i]->get_key() < k)
            cur = cur->level_next[i];
        if(cur->level_next[i] != nullptr && cur->level_next[i]->get_key() == k) {
            if(!find)
                find = true;
            tmp = cur->level_next[i];
            cur->level_next[i] = tmp->level_next[i];
        }
    }

    if(!find) { // 如果没有这个键，直接退出
        std::cout << "[DELETE ERROR] Key " << k << " is not exist!" << std::endl;
        return false;
    }
    delete tmp;
    std::cout << "[INFO] Delete key " << k << " succeeded!" << std::endl;
    return true;
}

template<typename K, typename V>
bool SkipList<K, V>::dump_file(char *filename) const {
    std::ofstream ofs(filename);
    if(!ofs.is_open()) {
        std::cout << "[DUMPFILE ERROR] the file " << filename << " open error!" << std::endl;
        return false;
    }
    ofs << typeid(K).name() << '\n' << typeid(V).name() << std::endl; // 保存键值对的类型
    Node<K, V> *cur = head->level_next[0];
    while(cur != nullptr) {
        ofs << cur->get_key() << separator << cur->get_value() << std::endl;
        cur = cur->level_next[0];
    }
    std::cout << "[INFO] dump data to file " << filename << " succeeded!" << std::endl;
    ofs.close();
    return true;
}

template<typename K, typename V>
bool SkipList<K, V>::load_file(char *filename) {
    std::ifstream ifs(filename);
    if(!ifs.good()) {
        std::cout << "[LOADFILE ERROR] the file " << filename << " is not exist!" << std::endl;
        return false;
    }
    // 类型检查
    std::string k_type, v_type;
    getline(ifs, k_type);
    getline(ifs,v_type);
    if(k_type != typeid(K).name() || v_type != typeid(V).name()) {
        std::cout << "[LOADFILE ERROR] The data type of key or value in this file is different from that of the skip list!" << std::endl;
        return false;
    }

    std::string str;
    K key;
    V value;
    while(getline(ifs, str)) {
        std::size_t pos = str.find(separator);
        if(pos == str.npos) { //没有找到分隔符，说明数据格式有问题，无法继续读取
            std::cout << "[LOADFILE ERROR] Data in wrong format!" << std::endl;
            return false;
        }
        key = boost::lexical_cast<K>(str.substr(0, pos));
        value = boost::lexical_cast<V>(str.substr(pos + 1));
        insert(key, value);
    }
    std::cout << "[INFO] load file " << filename << " succeeded!" << std::endl;
    ifs.close();
    return true;
}

#endif //SKIPLIST_SKIPLIST_H
