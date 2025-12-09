#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <vector>

struct Graph {
    std::vector<std::vector<std::size_t>> adj;
    std::vector<std::size_t> indegree;
};

Graph parse_graph(std::istream &in) {
    std::size_t n, m;
    if (!(in >> n >> m))
        throw std::runtime_error("invalid header (expected n m)");
    if (n == 0)
        throw std::runtime_error("n must be > 0");

    Graph g;
    g.adj.assign(n, {});
    g.indegree.assign(n, 0);

    for (std::size_t i = 0; i < m; ++i) {
        long u, v;
        if (!(in >> u >> v))
            throw std::runtime_error("invalid edge line");
        if (u < 1 || v < 1 || u > static_cast<long>(n) || v > static_cast<long>(n)) {
            throw std::runtime_error("vertex id out of range; use 1..n");
        }
        const std::size_t from = static_cast<std::size_t>(u - 1);
        const std::size_t to = static_cast<std::size_t>(v - 1);
        g.adj[from].push_back(to);
        ++g.indegree[to];
    }

    return g;
}

Graph load_graph_from(const std::string &path) {
    if (path == "-")
        return parse_graph(std::cin);
    std::ifstream fin(path);
    if (!fin)
        throw std::runtime_error("cannot open input file: " + path);
    return parse_graph(fin);
}

std::optional<std::vector<std::size_t>> topo_sort(const Graph &g, std::size_t &processed) {
    std::vector<std::size_t> indeg = g.indegree;
    std::queue<std::size_t> ready;
    for (std::size_t i = 0; i < indeg.size(); ++i) {
        if (indeg[i] == 0)
            ready.push(i);
    }

    std::vector<std::size_t> order;
    order.reserve(indeg.size());

    while (!ready.empty()) {
        std::size_t u = ready.front();
        ready.pop();
        order.push_back(u);
        for (std::size_t v : g.adj[u]) {
            if (--indeg[v] == 0)
                ready.push(v);
        }
    }

    processed = order.size();
    if (processed != g.adj.size())
        return std::nullopt;
    return order;
}

void print_topo_order(const std::vector<std::size_t> &order) {
    std::cout << "Topological order: ";
    for (std::size_t i = 0; i < order.size(); ++i) {
        if (i)
            std::cout << ' ';
        std::cout << (order[i] + 1);
    }
    std::cout << "\n";
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <graph.txt|->\n";
        std::cerr << "Input format: n m followed by m lines of edges u v." << std::endl;
        return 1;
    }

    try {
        Graph g = load_graph_from(argv[1]);
        std::size_t processed = 0;
        auto order = topo_sort(g, processed);
        if (!order) {
            std::cout << "Cycle detected (processed " << processed << "/" << g.adj.size() << ").\n";
            return 0;
        }
        print_topo_order(*order);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
