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

    int n, m, s;
    cin >> n >> m >> s;

    vector<vector<Edge>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }

    vector<int> dist(n + 1, INF);
    vector<int> cnt(n + 1, 0);      // 入队次数, 用于负环判定
    vector<bool> inq(n + 1, false); // 是否在队列中

    dist[s] = 0;
    queue<int> q;
    q.push(s);
    inq[s] = true;

    bool negative_cycle = false;

    while (!q.empty() && !negative_cycle) {
        int u = q.front();
        q.pop();
        inq[u] = false;

        for (const auto &e : g[u]) {
            int nd = dist[u] + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                if (!inq[e.to]) {
                    q.push(e.to);
                    inq[e.to] = true;
                    cnt[e.to]++;
                    // 一个点入队超过 n 次说明存在负环
                    if (cnt[e.to] > n) {
                        negative_cycle = true;
                        break;
                    }
                }
            }
        }
    }

    if (negative_cycle) {
        cout << "NEGATIVE CYCLE\n";
    } else {
        for (int i = 1; i <= n; ++i) {
            if (dist[i] == INF)
                cout << "INF ";
            else
                cout << dist[i] << ' ';
        }
        cout << '\n';
    }

    return 0;
}
