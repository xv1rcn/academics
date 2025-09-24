#pragma once

#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

template <typename T>
class ordered_list {
    struct Node {
        T value;
        Node *next;
        Node(const T &val, Node *nxt = nullptr) : value(val), next(nxt) {}
    };
    Node *head = nullptr;
    Node *tail = nullptr;
    size_t _size = 0;

    Node *merge_sort(Node *node, size_t len) {
        if (len <= 1) return node;
        size_t mid = len / 2;
        Node *left = node, *right = node;
        for (size_t i = 0; i < mid - 1; ++i) right = right->next;
        Node *rhead = right->next;
        right->next = nullptr;
        left = merge_sort(left, mid);
        rhead = merge_sort(rhead, len - mid);
        Node dummy(T{});
        Node *tail = &dummy;
        while (left && rhead) {
            if (left->value < rhead->value) {
                tail->next = left;
                left = left->next;
            } else {
                tail->next = rhead;
                rhead = rhead->next;
            }
            tail = tail->next;
        }
        tail->next = left ? left : rhead;
        return dummy.next;
    }

public:
    ordered_list() = default;
    ordered_list(const ordered_list &other) : head(nullptr), tail(nullptr), _size(0) {
        for (Node *cur = other.head; cur; cur = cur->next) push_back(cur->value);
    }
    ~ordered_list() { clear(); }

    ordered_list &operator=(const ordered_list &other) {
        if (this == &other) return *this;
        clear();
        for (Node *cur = other.head; cur; cur = cur->next) push_back(cur->value);
        return *this;
    }

    T &operator[](size_t idx) {
        if (idx >= _size) throw "Index out of range";
        Node *cur = head;
        for (size_t i = 0; i < idx; ++i) cur = cur->next;
        return cur->value;
    }
    const T &operator[](size_t idx) const {
        if (idx >= _size) throw "Index out of range";
        Node *cur = head;
        for (size_t i = 0; i < idx; ++i) cur = cur->next;
        return cur->value;
    }

    class iterator {
        Node *ptr;

    public:
        iterator(Node *p) : ptr(p) {}

        T &operator*() { return ptr->value; }

        iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }

        bool operator!=(const iterator &other) const { return ptr != other.ptr; }
    };

    class const_iterator {
        Node *ptr;

    public:
        const_iterator(Node *p) : ptr(p) {}

        const T &operator*() const { return ptr->value; }

        const_iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }

        bool operator!=(const const_iterator &other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }

    void clear() {
        Node *cur = head;
        while (cur) {
            Node *tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head = nullptr;
        tail = nullptr;
        _size = 0;
    }

    bool contains(const T &val) const { return find(val) != _size; }

    bool empty() const { return _size == 0; }

    void erase(size_t idx) {
        if (idx >= _size) return;
        Node **cur = &head;
        for (size_t i = 0; i < idx; ++i) cur = &((*cur)->next);
        Node *tmp = *cur;
        *cur = tmp->next;
        if (tmp == tail) {
            if (idx == 0)
                tail = nullptr;
            else {
                Node *tcur = head;
                for (size_t i = 1; i < _size - 1; ++i) tcur = tcur->next;
                tail = tcur;
            }
        }
        delete tmp;
        --_size;
    }

    size_t find(const T &val) const {
        size_t idx = 0;
        for (Node *cur = head; cur; cur = cur->next, ++idx) {
            if (cur->value == val) return idx;
        }
        return _size;
    }

    void insert(size_t pos, const T &val) {
        if (pos > _size) pos = _size;
        if (pos == 0) {
            Node *new_node = new Node(val, head);
            head = new_node;
            if (_size == 0) tail = new_node;
        } else if (pos == _size) {
            push_back(val);
            return;
        } else {
            Node *cur = head;
            for (size_t i = 0; i < pos - 1; ++i) cur = cur->next;
            cur->next = new Node(val, cur->next);
        }
        ++_size;
    }

    void merge(const ordered_list &other) {
        size_t old_size = _size;
        Node dummy(T{});
        Node *mtail = &dummy;
        Node *l1 = head, *l2 = other.head;
        while (l1 && l2) {
            if (l1->value < l2->value) {
                mtail->next = new Node(l1->value);
                l1 = l1->next;
            } else {
                mtail->next = new Node(l2->value);
                l2 = l2->next;
            }
            mtail = mtail->next;
        }
        while (l1) {
            mtail->next = new Node(l1->value);
            mtail = mtail->next;
            l1 = l1->next;
        }
        while (l2) {
            mtail->next = new Node(l2->value);
            mtail = mtail->next;
            l2 = l2->next;
        }
        clear();
        head = dummy.next;
        tail = head;
        if (tail) while (tail->next) tail = tail->next;
        _size = old_size + other._size;
    }

    void ordered_insert(const T &val) {
        Node **cur = &head;
        while (*cur && (*cur)->value < val) cur = &((*cur)->next);
        *cur = new Node(val, *cur);
        if ((*cur)->next == nullptr) tail = *cur;
        ++_size;
    }

    void push_back(const T &val) {
        Node *new_node = new Node(val);
        if (!head) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++_size;
    }

    void remove(const T &val) {
        size_t idx = find(val);
        if (idx != _size) erase(idx);
    }

    void resize(size_t new_size) {
        if (new_size == _size) return;
        if (new_size < _size) {
            if (new_size == 0) {
                clear();
                return;
            }
            Node *cur = head;
            for (size_t i = 1; i < new_size; ++i) cur = cur->next;
            Node *to_delete = cur->next;
            cur->next = nullptr;
            tail = cur;
            while (to_delete) {
                Node *tmp = to_delete;
                to_delete = to_delete->next;
                delete tmp;
            }
            _size = new_size;
        } else {
            if (!tail) {
                for (size_t i = 0; i < new_size; ++i) {
                    Node *new_node = new Node(T{});
                    if (!head) {
                        head = tail = new_node;
                    } else {
                        tail->next = new_node;
                        tail = new_node;
                    }
                }
                _size = new_size;
            } else {
                Node **cur = &tail->next;
                for (size_t i = _size; i < new_size; ++i) {
                    *cur = new Node(T{});
                    tail = *cur;
                    cur = &((*cur)->next);
                }
                _size = new_size;
            }
        }
    }

    size_t size() const { return _size; }

    void sort() {
        if (!head || !head->next) return;
        head = merge_sort(head, _size);
        tail = head;
        if (tail)
            while (tail->next) tail = tail->next;
    }
};

#endif // ORDERED_LIST_H
