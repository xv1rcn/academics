#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

constexpr int INF = 2'000'000'000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    // 邻接矩阵, g[u][v] = w, INF 表示无边
    vector g(n + 1, vector<int>(n + 1, INF));
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u][v] = min(g[u][v], w); // 重边取最小
    }

    vector<int> dist(n + 1, INF);
    vector<bool> vis(n + 1, false);
    dist[s] = 0;

    // O(V^2) 贪心: 每次从未访问点中选 dist 最小的进行松弛
    for (int i = 1; i <= n; ++i) {
        // 找到当前未访问且 dist 最小的点
        int u = 0;
        for (int v = 1; v <= n; ++v)
            if (!vis[v] && (u == 0 || dist[v] < dist[u])) u = v;

        if (dist[u] == INF) break; // 剩余点均不可达
        vis[u] = true;

        // 松弛 u 的所有出边
        for (int v = 1; v <= n; ++v)
            if (g[u][v] != INF && dist[v] > dist[u] + g[u][v]) dist[v] = dist[u] + g[u][v];
    }

    if (dist[t] == INF)
        cout << -1 << '\n';
    else
        cout << dist[t] << '\n';

    return 0;
}
