#pragma once

#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

template <typename T>
class ordered_array {
    T *data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

    void merge_sort(size_t left, size_t right, T* temp) {
        if (right - left <= 1) return;
        size_t mid = left + (right - left) / 2;
        merge_sort(left, mid, temp);
        merge_sort(mid, right, temp);
        size_t i = left, j = mid, k = left;
        while (i < mid && j < right) {
            if (data[i] < data[j]) temp[k++] = data[i++];
            else temp[k++] = data[j++];
        }
        while (i < mid) temp[k++] = data[i++];
        while (j < right) temp[k++] = data[j++];
        for (size_t t = left; t < right; ++t) data[t] = temp[t];
    }

public:
    ordered_array() : data(nullptr), _size(0), _capacity(0) {}
    explicit ordered_array(const size_t &size) : data(nullptr), _size(0), _capacity(size) {
        if (size) data = new T[size]{};
    }
    ordered_array(const ordered_array &other) : data(nullptr), _size(other._size), _capacity(other._capacity) {
        if (_capacity) {
            data = new T[_capacity]{};
            for (size_t i = 0; i < _size; ++i) data[i] = other.data[i];
        }
    }
    ~ordered_array() { delete[] data; data = nullptr; _size = 0; _capacity = 0; }

    ordered_array &operator=(const ordered_array &other) {
        if (this == &other) return *this;
        delete[] data;
        _size = other._size;
        _capacity = other._capacity;
        if (_capacity) {
            data = new T[_capacity]{};
            for (size_t i = 0; i < _size; ++i) data[i] = other.data[i];
        } else {
            data = nullptr;
        }
        return *this;
    }

    T &operator[](const size_t &index) {
        if (index >= _size) throw "Array index out of range";
        return data[index];
    }
    const T &operator[](const size_t &index) const {
        if (index >= _size) throw "Array index out of range";
        return data[index];
    }

    T *begin() { return data; }
    T *end() { return data + _size; }
    const T *begin() const { return data; }
    const T *end() const { return data + _size; }

    void clear() {
        delete[] data;
        data = nullptr;
        _size = 0;
        _capacity = 0;
    }

    bool contains(const T &val) const { return find(val) != _size; }

    bool empty() const { return _size == 0; }

    void erase(size_t idx) {
        if (idx >= _size) return;
        for (size_t i = idx; i < _size - 1; ++i) data[i] = data[i + 1];
        resize(_size - 1);
    }

    size_t find(const T &val) const {
        size_t l = 0, r = _size;
        while (l < r) {
            size_t m = (l + r) / 2;
            if (data[m] < val)
                l = m + 1;
            else if (data[m] > val)
                r = m;
            else
                return m;
        }
        return _size;
    }

    void insert(size_t pos, const T &val) {
        if (pos > _size) pos = _size;
        if (_size == _capacity) {
            size_t new_cap = _capacity ? _capacity * 2 : 8;
            T *new_data = new T[new_cap]{};
            for (size_t i = 0; i < pos; ++i) new_data[i] = data[i];
            new_data[pos] = val;
            for (size_t i = pos; i < _size; ++i) new_data[i + 1] = data[i];
            delete[] data;
            data = new_data;
            _capacity = new_cap;
        } else {
            for (size_t i = _size; i > pos; --i) data[i] = data[i - 1];
            data[pos] = val;
        }
        ++_size;
    }

    void merge(const ordered_array &other) {
        if (!other._size) return;
        T *new_data = new T[_size + other._size]{};
        T *pt1 = data, *pt2 = other.data, *pt = new_data;
        while (pt1 < data + _size && pt2 < other.data + other._size) {
            if (*pt1 < *pt2)
                *pt++ = *pt1++;
            else
                *pt++ = *pt2++;
        }
        while (pt1 < data + _size) *pt++ = *pt1++;
        while (pt2 < other.data + other._size) *pt++ = *pt2++;
        delete[] data;
        data = new_data;
        _size += other._size;
        _capacity = _size;
    }

    void ordered_insert(const T &val) {
        size_t l = 0, r = _size;
        while (l < r) {
            size_t m = (l + r) / 2;
            if (data[m] < val)
                l = m + 1;
            else if (data[m] > val)
                r = m;
            else
                l = r = m;
        }
        insert(l, val);
    }

    void push_back(const T &val) {
        insert(_size, val);
    }

    void remove(const T &val) {
        size_t idx = find(val);
        if (idx != _size) erase(idx);
    }

    void resize(const size_t &size) {
        if (size == _size) return;
        if (size == 0) {
            clear();
            return;
        }
        if (size > _capacity) {
            size_t new_cap = size;
            T *new_data = new T[new_cap]{};
            for (size_t i = 0; i < _size; ++i) new_data[i] = data[i];
            delete[] data;
            data = new_data;
            _capacity = new_cap;
        }
        _size = size;
    }

    size_t size() const { return _size; }

    void sort() {
        if (_size <= 1) return;
        T* temp = new T[_size]{};
        merge_sort(0, _size, temp);
        delete[] temp;
    }
};

#endif // ORDERED_ARRAY_H
