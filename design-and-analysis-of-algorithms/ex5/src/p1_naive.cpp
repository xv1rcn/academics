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
    vector<int> ans;

    for (int i = 0; i + m <= n; ++i) {
        bool match = true;
        for (int j = 0; j < m; ++j) {
            if (S[i + j] != P[j]) {
                match = false;
                break;
            }
        }
        if (match) ans.push_back(i);
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
