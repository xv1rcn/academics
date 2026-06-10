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

    int n, m, k;
    cin >> n >> m >> k;

    int S = 0, T = n + m + 1;
    int N = T + 1;

    vector<vector<Edge>> g(N);
    auto add_edge = [&](int u, int v, long long cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    };

    for (int i = 1; i <= n; ++i) add_edge(S, i, 1);
    for (int j = 1; j <= m; ++j) add_edge(n + j, T, 1);

    for (int e = 0; e < k; ++e) {
        int u, v;
        cin >> u >> v;
        add_edge(u, n + v, 1);
    }

    // Dinic max-flow on unit-capacity network
    vector<int> level(N), ptr(N);
    long long max_match = 0;

    auto bfs = [&]() -> bool {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(S);
        level[S] = 0;
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
        return level[T] != -1;
    };

    function<long long(int, long long)> dfs = [&](int u, long long pushed) -> long long {
        if (u == T) return pushed;
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
            long long pushed = dfs(S, numeric_limits<long long>::max());
            if (pushed == 0) break;
            max_match += pushed;
        }
    }

    cout << max_match << '\n';

    // Minimum vertex cover via Konig's theorem:
    // unreachable left nodes + reachable right nodes in residual graph
    vector<bool> vis(N, false);
    queue<int> q;
    q.push(S);
    vis[S] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const Edge &e : g[u]) {
            if (e.cap > 0 && !vis[e.to]) {
                vis[e.to] = true;
                q.push(e.to);
            }
        }
    }

    vector<int> cover;
    for (int i = 1; i <= n; ++i)
        if (!vis[i]) cover.push_back(i);
    for (int j = 1; j <= m; ++j)
        if (vis[n + j]) cover.push_back(-j);

    cout << cover.size() << '\n';
    for (size_t i = 0; i < cover.size(); ++i) {
        if (i > 0) cout << ' ';
        if (cover[i] > 0)
            cout << 'L' << cover[i];
        else
            cout << 'R' << -cover[i];
    }
    cout << '\n';
}
