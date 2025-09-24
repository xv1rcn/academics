#pragma once

#ifndef ORDERED_ARRAY_STL_H
#define ORDERED_ARRAY_STL_H

#include <algorithm>
#include <stdexcept>
#include <vector>

template <typename T>
class ordered_array_stl {
    std::vector<T> data;

public:
    ordered_array_stl() = default;
    explicit ordered_array_stl(size_t size) : data(size) {}
    ordered_array_stl(const ordered_array_stl &other) = default;
    ~ordered_array_stl() = default;

    ordered_array_stl &operator=(const ordered_array_stl &other) = default;

    T &operator[](size_t index) {
        if (index >= data.size()) throw std::out_of_range("Index out of range");
        return data[index];
    }
    const T &operator[](size_t index) const {
        if (index >= data.size()) throw std::out_of_range("Index out of range");
        return data[index];
    }

    typename std::vector<T>::iterator begin() { return data.begin(); }
    typename std::vector<T>::iterator end() { return data.end(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T>::const_iterator end() const { return data.end(); }

    void clear() { data.clear(); }

    bool contains(const T &val) const { return std::binary_search(data.begin(), data.end(), val); }

    bool empty() const { return data.empty(); }

    void erase(size_t idx) {
        if (idx < data.size()) data.erase(data.begin() + idx);
    }

    size_t find(const T &val) const {
        auto pos = std::lower_bound(data.begin(), data.end(), val);
        if (pos != data.end() && *pos == val) return std::distance(data.begin(), pos);
        return data.size();
    }

    void insert(size_t pos, const T &val) {
        if (pos > data.size()) pos = data.size();
        data.insert(data.begin() + pos, val);
    }

    void merge(const ordered_array_stl &other) {
        std::vector<T> result;
        result.reserve(data.size() + other.data.size());
        std::merge(data.begin(), data.end(), other.data.begin(), other.data.end(), std::back_inserter(result));
        data = std::move(result);
    }
    
    void ordered_insert(const T &val) {
        auto pos = std::lower_bound(data.begin(), data.end(), val);
        data.insert(pos, val);
    }

    void push_back(const T &val) {
        data.push_back(val);
    }

    void remove(const T &val) {
        auto pos = std::lower_bound(data.begin(), data.end(), val);
        if (pos != data.end() && *pos == val) data.erase(pos);
    }

    void resize(size_t new_size) { data.resize(new_size); }

    size_t size() const { return data.size(); }

    void sort() { std::sort(data.begin(), data.end()); }
};

#endif // ORDERED_ARRAY_STL_H
