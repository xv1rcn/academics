#include <iostream>
#include <set>
#include <unordered_map>

#include "tree.h"

template <typename T> Tree<T>::Tree(const std::vector<std::pair<T, T>> &pairs) {
    std::unordered_map<T, TreeNode<T> *> nodes;
    std::set<T> children;
    for (const auto &p : pairs) {
        if (!nodes.count(p.first))
            nodes[p.first] = new TreeNode<T>(p.first);
        if (!nodes.count(p.second))
            nodes[p.second] = new TreeNode<T>(p.second);
        TreeNode<T> *parent = nodes[p.first];
        TreeNode<T> *child = nodes[p.second];
        if (!parent->firstChild) {
            parent->firstChild.reset(child);
        } else {
            TreeNode<T> *sib = parent->firstChild.get();
            while (sib->nextSibling)
                sib = sib->nextSibling.get();
            sib->nextSibling.reset(child);
        }
        children.insert(p.second);
    }
    for (const auto &kv : nodes) {
        if (!children.count(kv.first)) {
            root.reset(kv.second);
            break;
        }
    }
}

template <typename T> const TreeNode<T> *Tree<T>::getRoot() const {
    return root.get();
}

template <typename T> void Tree<T>::preOrder() const {
    preOrder(root.get());
    std::cout << std::endl;
}

template <typename T> void Tree<T>::preOrder(TreeNode<T> *node) const {
    if (!node)
        return;
    std::cout << node->data << " ";
    preOrder(node->firstChild.get());
    preOrder(node->nextSibling.get());
}

template <typename T> void Tree<T>::postOrder() const {
    postOrder(root.get());
    std::cout << std::endl;
}

template <typename T> void Tree<T>::postOrder(TreeNode<T> *node) const {
    if (!node)
        return;
    postOrder(node->firstChild.get());
    std::cout << node->data << " ";
    postOrder(node->nextSibling.get());
}

template <typename T> int Tree<T>::getDegree(const TreeNode<T> *node) const {
    int deg = 0;
    auto child = node->firstChild.get();
    while (child) {
        ++deg;
        child = child->nextSibling.get();
    }
    return deg;
}

template <typename T> void Tree<T>::printDegrees() const {
    printDegrees(root.get());
}

template <typename T> void Tree<T>::printDegrees(TreeNode<T> *node) const {
    if (!node)
        return;
    std::cout << "结点 " << node->data << " 的度: " << getDegree(node)
              << std::endl;
    printDegrees(node->firstChild.get());
    printDegrees(node->nextSibling.get());
}

template <typename T> int Tree<T>::getHeight() const {
    return getHeight(root.get());
}

template <typename T> int Tree<T>::getHeight(const TreeNode<T> *node) const {
    if (!node)
        return 0;
    int maxH = 0;
    for (auto child = node->firstChild.get(); child;
         child = child->nextSibling.get()) {
        int h = getHeight(child);
        if (h > maxH)
            maxH = h;
    }
    return maxH + 1;
}

template <typename T> void Tree<T>::printPaths() const {
    std::vector<T> path;
    printPaths(root.get(), path);
}

template <typename T>
void Tree<T>::printPaths(TreeNode<T> *node, std::vector<T> &path) const {
    if (!node)
        return;
    path.push_back(node->data);
    if (!node->firstChild) {
        std::cout << "路径: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i + 1 == path.size() ? "\n" : " -> ");
        }
    } else {
        for (auto child = node->firstChild.get(); child;
             child = child->nextSibling.get()) {
            printPaths(child, path);
        }
    }
    path.pop_back();
}

template class Tree<int>;
