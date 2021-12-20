//
// Created by hunz1 on 2021/12/19.
//

#ifndef __SKIPLIST_NODE_H__
#define __SKIPLIST_NODE_H__

template <typename K, typename V>
class Node {
private:
    K key;
    V value;

public:
    Node<K, V>** level_next;  // 存储当前节点在每一层的下一个节点地址。
    int level;

    Node();

    Node(K k, V v, int l);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_value(V v);
};

template <typename K, typename V>
Node<K, V>::Node() : key(K()), value(V()), level(0) {
    level_next = nullptr;
}

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, const int l) : key(k), value(v), level(l) {
    level_next = new Node<K, V>*[level + 1];  // 申请level + 1个(从0到level)以Node*为元素的数组
    for (int i = 0; i < l; ++i)
        level_next[i] = nullptr;
}

template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
}

template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
}

template <typename K, typename V>
Node<K, V>::~Node() {
    delete[] level_next;
}

template <typename K, typename V>
void Node<K, V>::set_value(V v) {
    value = v;
}

#endif  // __SKIPLIST_NODE_H__
