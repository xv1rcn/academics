#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

constexpr int INF = 2'000'000'000;

struct Edge {
    int to, weight;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, s, t;
    cin >> n >> m >> k >> s >> t;

    vector<vector<Edge>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    // dist[v][used]: 从 s 到 v, 用了 used 次免费机会的最短距离
    vector dist(n + 1, vector<int>(k + 1, INF));
    dist[s][0] = 0;

    // 小根堆: (距离, 顶点, 已用免费次数)
    using State = tuple<int, int, int>;
    priority_queue<State, vector<State>, greater<>> pq;
    pq.push({0, s, 0});

    while (!pq.empty()) {
        auto [d, u, used] = pq.top();
        pq.pop();

        if (d != dist[u][used]) continue; // 惰性删除

        for (const auto &e : g[u]) {
            // 转移一: 正常走这条边, 付费
            int nd = d + e.weight;
            if (nd < dist[e.to][used]) {
                dist[e.to][used] = nd;
                pq.push({nd, e.to, used});
            }
            // 转移二: 使用免费机会
            if (used < k && d < dist[e.to][used + 1]) {
                dist[e.to][used + 1] = d;
                pq.push({d, e.to, used + 1});
            }
        }
    }

    // 答案: 到达 t 的所有状态中的最小值
    int ans = INF;
    for (int j = 0; j <= k; ++j) ans = min(ans, dist[t][j]);

    cout << ans << '\n';

    return 0;
}
