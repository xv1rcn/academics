#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bst.h"

namespace {

    template <typename T>
    double compute_avg_path(const bst<T> &tree) {
        if (tree.empty())
            return 0.0;
        long long sum = 0, cnt = 0;
        tree.traverse([&](const T &, int depth) {
            sum += depth;
            ++cnt;
        });
        if (cnt == 0)
            return 0.0;
        return static_cast<double>(sum) / static_cast<double>(cnt);
    }

    template <typename T>
    void print_path(const std::vector<T> &path) {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i + 1 != path.size())
                std::cout << " -> ";
        }
        if (path.empty()) {
            std::cout << "(empty)";
        }
        std::cout << '\n';
    }

    std::vector<int> load_data(const std::string &path) {
        std::istream *in = &std::cin;
        std::ifstream fin;
        if (path != "-") {
            fin.open(path);
            if (!fin)
                throw std::runtime_error("cannot open input file: " + path);
            in = &fin;
        }

        int n = 0;
        if (!(*in >> n))
            throw std::runtime_error("invalid header (expected count)");
        if (n < 0)
            throw std::runtime_error("count must be non-negative");

        std::vector<int> data(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) {
            if (!(*in >> data[static_cast<size_t>(i)])) {
                throw std::runtime_error("invalid data at index " + std::to_string(i));
            }
        }
        return data;
    }

} // namespace

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.txt|->\n";
        std::cerr << "Input format: n followed by n integers (use '-' for stdin)." << std::endl;
        return 1;
    }

    try {
        std::vector<int> data = load_data(argv[1]);

        bst<int> tree;
        tree.build(data);

        const auto inorderSeq = tree.inorder();
        std::cout << "In-order traversal (ascending): ";
        for (size_t i = 0; i < inorderSeq.size(); ++i) {
            std::cout << inorderSeq[i] << (i + 1 == inorderSeq.size() ? '\n' : ' ');
        }

        double asl = compute_avg_path(tree);
        std::cout << "Average successful search length (ASL): " << asl << "\n";

        while (true) {
            std::cout << "Search key (q to quit): ";
            std::string token;
            if (!(std::cin >> token))
                break;
            if (token == "q" || token == "Q")
                break;
            int key = std::stoi(token);

            bst<int>::find_result res = tree.find(key);

            std::cout << "Path: ";
            print_path(res.path);

            if (res.ptr)
                std::cout << "Found key " << key << "\n";
            else
                std::cout << "Key " << key << " not found\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
