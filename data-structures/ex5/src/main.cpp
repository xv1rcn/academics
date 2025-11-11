#include <iostream>
#include <utility>
#include <vector>

#include "tree.h"

int main() {
    std::vector<std::pair<int, int>> pairs = {{1, 2}, {1, 3}, {2, 4},
                                              {2, 5}, {3, 6}, {3, 7}};
    Tree<int> tree(pairs);

    std::cout << "先根遍历: ";
    tree.preOrder();

    std::cout << "后根遍历: ";
    tree.postOrder();

    std::cout << "每个结点的度: " << std::endl;
    tree.printDegrees();

    std::cout << "树的高度: " << tree.getHeight() << std::endl;

    std::cout << "根到每个叶子的路径: " << std::endl;
    tree.printPaths();

    return 0;
}
