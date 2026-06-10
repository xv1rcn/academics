#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS from root 1 to get parent[] and level-order
    vector<int> parent(n + 1, 0);
    vector<int> order;
    order.reserve(n);
    queue<int> q;
    q.push(1);
    parent[1] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                q.push(v);
            }
        }
    }

    // Compute subtree sizes and stable status in reverse level-order
    vector<int> sz(n + 1, 1);
    vector<bool> stable(n + 1, false);

    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        if (parent[u] != -1) { sz[parent[u]] += sz[u]; }
    }

    for (int i = 1; i <= n; i++) { stable[i] = (sz[i] % 2 == 0); }

    // Bottom-up DP: compute stable component sizes
    vector<int> comp(n + 1, 0);
    long long ans = 0;

    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        if (stable[u]) {
            int cur = 1;
            for (int v : adj[u]) {
                if (v != parent[u]) {
                    if (stable[v]) { cur += comp[v]; }
                }
            }
            comp[u] = cur;
        } else {
            for (int v : adj[u]) {
                if (v != parent[u]) {
                    if (stable[v]) { ans += (long long)comp[v] * (comp[v] - 1); }
                }
            }
        }
    }

    if (stable[1]) { ans += (long long)comp[1] * (comp[1] - 1); }

    cout << ans << '\n';
    return 0;
}
