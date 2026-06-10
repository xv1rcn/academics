#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct Node {
    int next[26];
    int fail;
    int out;
    vector<int> ends;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> patterns(n);
    for (int i = 0; i < n; ++i) cin >> patterns[i];
    string S;
    cin >> S;

    vector<Node> trie(1);
    trie[0].out = -1;
    for (int k = 0; k < 26; ++k) trie[0].next[k] = -1;

    for (int i = 0; i < n; ++i) {
        const string &pat = patterns[i];
        int cur = 0;
        for (char ch : pat) {
            int c = ch - 'a';
            if (trie[cur].next[c] == -1) {
                trie[cur].next[c] = (int)trie.size();
                trie.emplace_back();
                int idx = (int)trie.size() - 1;
                for (int k = 0; k < 26; ++k) trie[idx].next[k] = -1;
                trie[idx].out = -1;
            }
            cur = trie[cur].next[c];
        }
        trie[cur].ends.push_back(i);
    }

    queue<int> q;
    trie[0].fail = 0;
    for (int c = 0; c < 26; ++c) {
        int nxt = trie[0].next[c];
        if (nxt != -1) {
            trie[nxt].fail = 0;
            q.push(nxt);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int c = 0; c < 26; ++c) {
            int v = trie[u].next[c];
            if (v != -1) {
                int f = trie[u].fail;
                while (f != 0 && trie[f].next[c] == -1) f = trie[f].fail;
                if (trie[f].next[c] != -1) f = trie[f].next[c];
                trie[v].fail = f;
                q.push(v);
            }
        }
    }

    vector<int> count(n, 0);
    int cur = 0;
    for (char ch : S) {
        int c = ch - 'a';

        while (cur != 0 && trie[cur].next[c] == -1) cur = trie[cur].fail;
        if (trie[cur].next[c] != -1) cur = trie[cur].next[c];

        for (int temp = cur; temp != 0; temp = trie[temp].fail) {
            for (int idx : trie[temp].ends) ++count[idx];
        }
    }

    for (int i = 0; i < n; ++i) cout << count[i] << '\n';
}
