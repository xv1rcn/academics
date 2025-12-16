#pragma once

#ifndef BST_H
#define BST_H

#include <memory>
#include <utility>
#include <vector>

template <typename T>
class bst {
public:
    struct find_result {
        const T *ptr{nullptr};
        std::vector<T> path;
    };

    bst() = default;
    bst(const bst &) = delete;
    bst &operator=(const bst &) = delete;
    bst(bst &&) = default;
    bst &operator=(bst &&) = default;
    ~bst() = default;

    void insert(const T &key) { insert(root_, key); }

    void build(const std::vector<T> &keys) {
        for (const T &v : keys)
            insert(v);
    }

    std::vector<T> inorder() const {
        std::vector<T> out;
        inorder(root_, out);
        return out;
    }

    find_result find(const T &key) const {
        std::vector<T> path;
        const node *cur = root_.get();
        while (cur) {
            path.push_back(cur->key);
            if (key == cur->key)
                return {&cur->key, std::move(path)};
            cur = (key < cur->key) ? cur->left.get() : cur->right.get();
        }
        return {nullptr, std::move(path)};
    }

    bool empty() const { return !root_; }

    template <typename Fn>
    void traverse(Fn &&fn) const {
        traverse(root_.get(), 1, std::forward<Fn>(fn));
    }

private:
    struct node {
        T key;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;

        explicit node(const T &k) : key(k) {}
    };

    std::unique_ptr<node> root_;

    bool insert(std::unique_ptr<node> &cur, const T &key) {
        if (!cur) {
            cur = std::make_unique<node>(key);
            return true;
        }
        if (key < cur->key)
            return insert(cur->left, key);
        if (key > cur->key)
            return insert(cur->right, key);
        return false;
    }

    void inorder(const std::unique_ptr<node> &cur, std::vector<T> &out) const {
        if (!cur)
            return;
        inorder(cur->left, out);
        out.push_back(cur->key);
        inorder(cur->right, out);
    }

    template <typename Fn>
    void traverse(const node *cur, int depth, Fn &&fn) const {
        if (!cur)
            return;
        std::forward<Fn>(fn)(cur->key, depth);
        traverse(cur->left.get(), depth + 1, std::forward<Fn>(fn));
        traverse(cur->right.get(), depth + 1, std::forward<Fn>(fn));
    }
};

#endif // BST_H
