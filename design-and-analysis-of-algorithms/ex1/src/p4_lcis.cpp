#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int &x : a) cin >> x;
    for (int &x : b) cin >> x;

    vector<int> dp(m, 0); // dp[j] = 以 b[j] 结尾的 LCIS 长度

    for (int i = 0; i < n; ++i) {
        int best = 0; // max{ dp[k] | k < j, b[k] < a[i] }
        for (int j = 0; j < m; ++j) {
            if (b[j] < a[i]) {
                best = max(best, dp[j]);
            } else if (a[i] == b[j]) {
                dp[j] = max(dp[j], best + 1);
            }
        }
    }

    cout << ranges::max(dp) << '\n';
}
