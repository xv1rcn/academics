#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

#include "ascii_printer.h"

void printAsciiBoxed(const BinaryTree<char> &tree, int pad) {
    using Node = BinaryTree<char>::Node;
    Node *root = tree.getRoot();
    if (!root) {
        std::cout << "╔══╗\n";
        std::cout << "║  ║\n";
        std::cout << "╚══╝\n";
        return;
    }

    std::function<int(const Node *)> height = [&](const Node *n) -> int {
        if (!n)
            return 0;
        return 1 + std::max(height(n->left.get()), height(n->right.get()));
    };
    int h = height(root);

    int rows = h * 2 - 1;
    int colsCells = (1 << h) * 2;

    std::vector<std::vector<const Node *>> levels(h);
    std::queue<const Node *> q;
    q.push(root);
    for (int lvl = 0; lvl < h; ++lvl) {
        int cnt = 1 << lvl;
        for (int i = 0; i < cnt; ++i) {
            const Node *n = nullptr;
            if (!q.empty()) {
                n = q.front();
                q.pop();
            }
            levels[lvl].push_back(n);
            if (n) {
                q.push(n->left.get());
                q.push(n->right.get());
            } else {
                q.push(nullptr);
                q.push(nullptr);
            }
        }
    }

    std::vector<std::vector<std::string>> gridCells(rows, std::vector<std::string>(colsCells, " "));
    std::vector<std::vector<int>> pos(h);

    for (int lvl = 0; lvl < h; ++lvl) {
        int nodes = (int)levels[lvl].size();
        int gap = colsCells / (nodes * 2);
        pos[lvl].resize(nodes);
        for (int i = 0; i < nodes; ++i) {
            int col = (2 * i + 1) * gap;
            int row = lvl * 2;
            pos[lvl][i] = col;
            const Node *node = levels[lvl][i];
            if (node && row >= 0 && row < rows && col >= 0 && col < colsCells) {
                gridCells[row][col] = std::string(1, node->val);
            }
        }
    }

    for (int lvl = 0; lvl < h - 1; ++lvl) {
        int nodes = (int)levels[lvl].size();
        int connRow = lvl * 2 + 1;
        for (int i = 0; i < nodes; ++i) {
            const Node *node = levels[lvl][i];
            if (!node)
                continue;
            int pcol = pos[lvl][i];
            int leftIdx = 2 * i;
            int rightIdx = 2 * i + 1;
            bool hasLeft = (leftIdx < (int)pos[lvl + 1].size() && levels[lvl + 1][leftIdx]);
            bool hasRight = (rightIdx < (int)pos[lvl + 1].size() && levels[lvl + 1][rightIdx]);
            int lcol = hasLeft ? pos[lvl + 1][leftIdx] : -1;
            int rcol = hasRight ? pos[lvl + 1][rightIdx] : -1;

            if (hasLeft && hasRight) {
                int start = std::min(lcol, pcol);
                if (start >= 0 && pcol < colsCells) {
                    gridCells[connRow][start] = "┌";
                    for (int c = start + 1; c < pcol; ++c)
                        gridCells[connRow][c] = "─";
                    gridCells[connRow][pcol] = "┴";
                }
                if (pcol >= 0 && rcol < colsCells) {
                    gridCells[connRow][pcol] = "┴";
                    for (int c = pcol + 1; c < rcol; ++c)
                        gridCells[connRow][c] = "─";
                    gridCells[connRow][rcol] = "┐";
                }
            } else if (hasLeft && !hasRight) {
                if (lcol >= 0 && pcol < colsCells) {
                    int start = std::min(lcol, pcol);
                    for (int c = start + 1; c < pcol; ++c)
                        gridCells[connRow][c] = "─";
                    gridCells[connRow][pcol] = "┘";
                    gridCells[connRow][lcol] = "┌";
                }
            } else if (!hasLeft && hasRight) {
                if (rcol >= 0 && pcol >= 0) {
                    for (int c = pcol + 1; c < rcol; ++c)
                        gridCells[connRow][c] = "─";
                    gridCells[connRow][pcol] = "└";
                    gridCells[connRow][rcol] = "┐";
                }
            }
        }
    }

    std::vector<std::string> lines;
    for (int r = 0; r < rows; ++r) {
        std::ostringstream oss;
        for (int c = 0; c < colsCells; ++c)
            oss << gridCells[r][c];
        lines.push_back(oss.str());
    }

    int innerWidthCells = colsCells;
    int totalInner = innerWidthCells + pad * 2;
    std::cout << "╔";
    for (int i = 0; i <= totalInner; ++i)
        std::cout << "═";
    std::cout << "╗\n";
    for (const auto &ln : lines) {
        std::cout << "║";
        for (int i = 0; i < pad; ++i)
            std::cout << ' ';
        std::cout << ln;
        for (int i = 0; i <= pad; ++i)
            std::cout << ' ';
        std::cout << "║\n";
    }
    std::cout << "╚";
    for (int i = 0; i <= totalInner; ++i)
        std::cout << "═";
    std::cout << "╝\n";
}
