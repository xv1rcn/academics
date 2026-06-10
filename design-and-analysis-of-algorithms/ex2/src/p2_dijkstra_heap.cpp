#include <iostream>
#include <queue>
#include <vector>

using namespace std;

constexpr int INF = 2'000'000'000;

struct Edge {
    int to, weight;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    // 邻接表
    vector<vector<Edge>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }

    vector<int> dist(n + 1, INF);
    dist[s] = 0;

    // 小根堆: (距离, 顶点编号)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u]) continue; // 惰性删除: 旧记录跳过

        // 松弛出边
        for (const auto &e : g[u]) {
            int nd = d + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push({nd, e.to});
            }
        }
    }

    if (dist[t] == INF)
        cout << -1 << '\n';
    else
        cout << dist[t] << '\n';

    return 0;
}
