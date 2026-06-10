#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int to, rev;
    long long cap, cost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    long long target_flow;
    cin >> n >> m >> s >> t >> target_flow;
    --s;
    --t;

    vector<vector<Edge>> g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long cap, cost;
        cin >> u >> v >> cap >> cost;
        --u;
        --v;
        g[u].push_back({v, (int)g[v].size(), cap, cost});
        g[v].push_back({u, (int)g[u].size() - 1, 0, -cost});
    }

    const long long INF = numeric_limits<long long>::max() / 4;
    vector<long long> h(n, 0);

    long long flow = 0, cost = 0;

    // SSP with Dijkstra + potentials
    while (flow < target_flow) {
        vector<long long> dist(n, INF);
        vector<int> parent(n, -1), pedge(n, -1);

        using P = pair<long long, int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        dist[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;

            for (int i = 0; i < (int)g[u].size(); ++i) {
                const Edge &e = g[u][i];
                if (e.cap > 0) {
                    long long nd = dist[u] + e.cost + h[u] - h[e.to];
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd;
                        parent[e.to] = u;
                        pedge[e.to] = i;
                        pq.push({nd, e.to});
                    }
                }
            }
        }

        if (dist[t] == INF) break;

        for (int i = 0; i < n; ++i)
            if (dist[i] < INF) h[i] += dist[i];

        long long bottle = target_flow - flow;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v], idx = pedge[v];
            bottle = min(bottle, g[u][idx].cap);
        }

        flow += bottle;
        cost += bottle * h[t];

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v], idx = pedge[v];
            g[u][idx].cap -= bottle;
            g[v][g[u][idx].rev].cap += bottle;
        }
    }

    if (flow < target_flow)
        cout << "-1\n";
    else
        cout << cost << '\n';
}
