#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "ascii_printer.h"
#include "binary_tree.h"
#include "cli.h"
#include "utils.h"

void printStatus(const BinaryTree<char> &tree) {
    printAsciiBoxed(tree, 1);

    auto root = tree.getRoot();
    if (!root) {
        std::cout << "当前树为空。\n\n";
        return;
    }

    std::string seq;
    std::function<void(const BinaryTree<char>::Node *)> buildPreorder = [&](const BinaryTree<char>::Node *n) {
        if (!n) {
            seq.push_back('#');
            return;
        }
        seq.push_back(n->val);
        buildPreorder(n->left.get());
        buildPreorder(n->right.get());
    };
    buildPreorder(root);

    std::cout << std::format("| toString: {}\n", seq);
    std::cout << std::format("| nodeCount: {}, height: {}\n\n", tree.nodeCount(), tree.height());
}

void printMenu() {
    std::cout << "1. 构建新二叉树\n";
    std::cout << "2. 显示遍历\n";
    std::cout << "3. 查找节点\n";
    std::cout << "0. 退出\n";
    std::cout << "输入选项编号并回车：";
}

void runCLI() {
    BinaryTree<char> bt;

    while (true) {
        printStatus(bt);
        printMenu();

        std::string choice;
        if (!getline(std::cin, choice))
            break;
        std::cout << '\n';
        if (choice.empty())
            continue;

        if (choice == "1") {
            std::cout << "请输入先序序列，虚结点用 # 指代：";
            std::string s;
            if (!getline(std::cin, s))
                break;
            auto t = parseCharTokens(s);
            if (isValidPreorder(t, '#')) {
                BinaryTree<char> tmp;
                tmp.buildFromPreorder(t, '#');
                bt = std::move(tmp);
                std::cout << "已构建。\n";
            } else {
                std::cout << "输入序列非法，未构建。\n";
            }
        } else if (choice == "2") {
            auto p = bt.preorder();
            auto i = bt.inorder();
            auto post = bt.postorder();
            auto lvl = bt.levelorder();
            std::cout << "先序: " << vecToStr(p) << '\n';
            std::cout << "中序: " << vecToStr(i) << '\n';
            std::cout << "后序: " << vecToStr(post) << '\n';
            std::cout << "层次: " << vecToStr(lvl) << '\n';
        } else if (choice == "3") {
            std::cout << "请输入要查找的字符：";
            std::string s;
            if (!getline(std::cin, s))
                break;
            if (s.empty()) {
                std::cout << "输入为空。\n";
                continue;
            }
            char q = s[0];
            auto node = bt.find(q);
            auto parent = bt.findParent(q);
            if (node)
                std::cout << "找到节点 '" << q << "'";
            else
                std::cout << "节点不存在: '" << q << "'";
            if (parent)
                std::cout << ", 父节点 = '" << parent->val << "'";
            else
                std::cout << ", 父节点 = null";
            std::cout << '\n';
        } else if (choice == "0") {
            std::cout << "退出程序。\n";
            break;
        } else {
            std::cout << "无效选项，请重试。\n";
        }

        {
            std::cout << "\n按回车键继续...\n";
            std::string t;
            std::getline(std::cin, t);
        }
    }

}
