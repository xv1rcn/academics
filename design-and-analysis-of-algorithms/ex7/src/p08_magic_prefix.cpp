#include <iostream>
#include <string>
#include <vector>
using namespace std;

void solve() {
    int N;
    cin >> N;

    // Flat-array trie: nxt[node * 26 + c] = child index, -1 if none
    vector<int> nxt(26, -1);

    for (int i = 0; i < N; ++i) {
        string s;
        cin >> s;
        int node = 0;
        for (char ch : s) {
            int c = ch - 'a';
            int idx = node * 26 + c;
            if (nxt[idx] == -1) {
                nxt[idx] = nxt.size() / 26;
                nxt.resize(nxt.size() + 26, -1);
            }
            node = nxt[idx];
        }
    }

    int num_nodes = nxt.size() / 26;
    vector<int> G(num_nodes, 0);

    // Bottom-up: children have higher indices than parents
    for (int v = num_nodes - 1; v >= 1; --v) {
        int base = v * 26;
        int xr = 0;
        for (int c = 0; c < 26; ++c) {
            int child = nxt[base + c];
            if (child != -1) { xr ^= G[child]; }
        }
        G[v] = 1 + xr;
    }

    // Root (0) is the empty prefix -- not playable.
    // The game is the XOR of its children'ssubtree games.
    int total = 0;
    for (int c = 0; c < 26; ++c) {
        int child = nxt[c];
        if (child != -1) { total ^= G[child]; }
    }

    cout << (total ? "XiaoLan" : "XiaoQiao") << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) solve();

    return 0;
}
