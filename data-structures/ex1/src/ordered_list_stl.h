#pragma once

#ifndef ORDERED_LIST_STL_H
#define ORDERED_LIST_STL_H

#include <algorithm>
#include <list>
#include <stdexcept>

template <typename T>
class ordered_list_stl {
    std::list<T> data;

public:
    ordered_list_stl() = default;
    explicit ordered_list_stl(size_t size) : data(size) {}
    ordered_list_stl(const ordered_list_stl &other) = default;
    ~ordered_list_stl() = default;

    ordered_list_stl &operator=(const ordered_list_stl &other) = default;

    T &operator[](size_t index) {
        if (index >= data.size()) throw std::out_of_range("Index out of range");
        auto it = data.begin();
        std::advance(it, index);
        return *it;
    }
    const T &operator[](size_t index) const {
        if (index >= data.size()) throw std::out_of_range("Index out of range");
        auto it = data.begin();
        std::advance(it, index);
        return *it;
    }

    typename std::list<T>::iterator begin() { return data.begin(); }
    typename std::list<T>::iterator end() { return data.end(); }
    typename std::list<T>::const_iterator begin() const { return data.begin(); }
    typename std::list<T>::const_iterator end() const { return data.end(); }

    void clear() { data.clear(); }

    bool contains(const T &val) const { return std::find(data.begin(), data.end(), val) != data.end(); }

    bool empty() const { return data.empty(); }

    void erase(size_t idx) {
        if (idx >= data.size()) return;
        auto it = data.begin();
        std::advance(it, idx);
        data.erase(it);
    }

    size_t find(const T &val) const {
        size_t idx = 0;
        for (auto it = data.begin(); it != data.end(); ++it, ++idx) {
            if (*it == val) return idx;
        }
        return data.size();
    }

    void insert(size_t pos, const T &val) {
        if (pos > data.size()) pos = data.size();
        auto it = data.begin();
        std::advance(it, pos);
        data.insert(it, val);
    }

    void merge(const ordered_list_stl &other) {
        std::list<T> other_copy = other.data;
        data.merge(other_copy);
    }

    void ordered_insert(const T &val) {
        auto pos = std::find_if(data.begin(), data.end(), [&](const T &x) { return x >= val; });
        data.insert(pos, val);
    }

    void push_back(const T &val) {
        data.push_back(val);
    }

    void remove(const T &val) {
        auto it = std::find(data.begin(), data.end(), val);
        if (it != data.end()) data.erase(it);
    }

    void resize(size_t new_size) { data.resize(new_size); }
    
    size_t size() const { return data.size(); }

    void sort() { data.sort(); }
};

#endif // ORDERED_LIST_STL_H
