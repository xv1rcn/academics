#pragma once
#include <memory>
#include <utility>
#include <vector>

template <typename T> struct TreeNode {
    T data;
    std::unique_ptr<TreeNode<T>> firstChild;
    std::unique_ptr<TreeNode<T>> nextSibling;
    TreeNode(const T &val) : data(val) {}
};

template <typename T> class Tree {
  private:
    std::unique_ptr<TreeNode<T>> root;
    TreeNode<T> *findNode(TreeNode<T> *node, const T &val);
    void preOrder(TreeNode<T> *node) const;
    void postOrder(TreeNode<T> *node) const;
    int getDegree(const TreeNode<T> *node) const;
    int getHeight(const TreeNode<T> *node) const;
    void printPaths(TreeNode<T> *node, std::vector<T> &path) const;
    void printDegrees(TreeNode<T> *node) const;

  public:
    Tree(const std::vector<std::pair<T, T>> &pairs);
    void preOrder() const;
    void postOrder() const;
    void printDegrees() const;
    int getHeight() const;
    void printPaths() const;
    const TreeNode<T> *getRoot() const;
};
