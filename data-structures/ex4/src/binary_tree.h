#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <algorithm>
#include <memory>
#include <queue>
#include <string>
#include <vector>

template <typename T>
class BinaryTree {
public:
    struct Node {
        T val;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        explicit Node(const T &v) : val(v), left(nullptr), right(nullptr) {}
    };

private:
    std::unique_ptr<Node> root;

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() = default;

    BinaryTree(const BinaryTree &) = delete;
    BinaryTree &operator=(const BinaryTree &) = delete;
    BinaryTree(BinaryTree &&other) noexcept = default;
    BinaryTree &operator=(BinaryTree &&other) noexcept = default;

    void buildFromPreorder(const std::vector<T> &tokens, const T &nullValue) {
        size_t idx = 0;
        root = buildFromPreorderRec(tokens, idx, nullValue);
    }

    std::vector<T> preorder() const {
        std::vector<T> out;
        preorderRec(root.get(), out);
        return out;
    }
    std::vector<T> inorder() const {
        std::vector<T> out;
        inorderRec(root.get(), out);
        return out;
    }
    std::vector<T> postorder() const {
        std::vector<T> out;
        postorderRec(root.get(), out);
        return out;
    }
    std::vector<T> levelorder() const { return levelOrderRec(root.get()); }

    int height() const { return heightRec(root.get()); }
    int nodeCount() const { return countRec(root.get()); }

    Node *find(const T &value) const { return findRec(root.get(), value); }
    Node *findParent(const T &value) const { return findParentRec(root.get(), nullptr, value); }

    Node *getRoot() const { return root.get(); }

private:
    std::unique_ptr<Node> buildFromPreorderRec(const std::vector<T> &tokens, size_t &idx, const T &nullValue) {
        if (idx >= tokens.size())
            return nullptr;
        T cur = tokens[idx++];
        if (cur == nullValue)
            return nullptr;
        auto node = std::make_unique<Node>(cur);
        node->left = buildFromPreorderRec(tokens, idx, nullValue);
        node->right = buildFromPreorderRec(tokens, idx, nullValue);
        return node;
    }

    void preorderRec(Node *node, std::vector<T> &out) const {
        if (!node)
            return;
        out.push_back(node->val);
        preorderRec(node->left.get(), out);
        preorderRec(node->right.get(), out);
    }

    void inorderRec(Node *node, std::vector<T> &out) const {
        if (!node)
            return;
        inorderRec(node->left.get(), out);
        out.push_back(node->val);
        inorderRec(node->right.get(), out);
    }

    void postorderRec(Node *node, std::vector<T> &out) const {
        if (!node)
            return;
        postorderRec(node->left.get(), out);
        postorderRec(node->right.get(), out);
        out.push_back(node->val);
    }

    std::vector<T> levelOrderRec(Node *node) const {
        std::vector<T> out;
        if (!node)
            return out;
        std::queue<Node *> q;
        q.push(node);
        while (!q.empty()) {
            Node *cur = q.front();
            q.pop();
            out.push_back(cur->val);
            if (cur->left)
                q.push(cur->left.get());
            if (cur->right)
                q.push(cur->right.get());
        }
        return out;
    }

    int heightRec(Node *node) const {
        if (!node)
            return 0;
        return 1 + std::max(heightRec(node->left.get()), heightRec(node->right.get()));
    }

    int countRec(Node *node) const {
        if (!node)
            return 0;
        return 1 + countRec(node->left.get()) + countRec(node->right.get());
    }

    Node *findRec(Node *node, const T &value) const {
        if (!node)
            return nullptr;
        if (node->val == value)
            return node;
        Node *l = findRec(node->left.get(), value);
        if (l)
            return l;
        return findRec(node->right.get(), value);
    }

    Node *findParentRec(Node *node, Node *parent, const T &value) const {
        if (!node)
            return nullptr;
        if (node->val == value)
            return parent;
        Node *l = findParentRec(node->left.get(), node, value);
        if (l)
            return l;
        return findParentRec(node->right.get(), node, value);
    }
};

#endif // BINARY_TREE_H
