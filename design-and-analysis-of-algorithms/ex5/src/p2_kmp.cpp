#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S, P;
    cin >> S >> P;

    int n = (int)S.size(), m = (int)P.size();

    vector<int> next(m);
    next[0] = 0;
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && P[i] != P[j]) j = next[j - 1];
        if (P[i] == P[j]) ++j;
        next[i] = j;
    }

    vector<int> ans;
    for (int i = 0, j = 0; i < n; ++i) {
        while (j > 0 && S[i] != P[j]) j = next[j - 1];
        if (S[i] == P[j]) ++j;
        if (j == m) {
            ans.push_back(i - m + 1);
            j = next[j - 1];
        }
    }

    if (ans.empty()) {
        cout << "-1\n";
    } else {
        for (size_t i = 0; i < ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
}
