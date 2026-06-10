#include <algorithm>
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

    long long max_flow = 0;
    const long long INF = numeric_limits<long long>::max();

    while (true) {
        vector<int> parent(n, -1), pedge(n, -1);
        queue<int> q;
        q.push(s);
        parent[s] = s;

        while (!q.empty() && parent[t] == -1) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < (int)g[u].size(); ++i) {
                const Edge &e = g[u][i];
                if (e.cap > 0 && parent[e.to] == -1) {
                    parent[e.to] = u;
                    pedge[e.to] = i;
                    q.push(e.to);
                }
            }
        }

        if (parent[t] == -1) break;

        long long bottle = INF;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v], idx = pedge[v];
            bottle = min(bottle, g[u][idx].cap);
        }

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v], idx = pedge[v];
            g[u][idx].cap -= bottle;
            g[v][g[u][idx].rev].cap += bottle;
        }

        max_flow += bottle;
    }

    cout << max_flow << '\n';
}
