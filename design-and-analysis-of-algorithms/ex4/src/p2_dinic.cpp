#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int to, rev;
    long long cap;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    --s;
    --t;

    vector<vector<Edge>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;
        --u;
        --v;
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    vector<int> level(n), ptr(n);
    long long max_flow = 0;

    auto bfs = [&]() -> bool {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge &e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    };

    function<long long(int, long long)> dfs = [&](int u, long long pushed) -> long long {
        if (u == t) return pushed;
        for (int &i = ptr[u]; i < (int)g[u].size(); ++i) {
            Edge &e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                long long tr = dfs(e.to, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    g[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    };

    while (bfs()) {
        fill(ptr.begin(), ptr.end(), 0);
        while (true) {
            long long pushed = dfs(s, numeric_limits<long long>::max());
            if (pushed == 0) break;
            max_flow += pushed;
        }
    }

    cout << max_flow << '\n';
}
