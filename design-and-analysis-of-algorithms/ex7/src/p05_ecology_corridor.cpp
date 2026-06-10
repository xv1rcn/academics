#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
using namespace std;

using LL = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // base interaction strength
    LL base = 0;
    for (int i = 0; i < n - 1; ++i) base += (LL)(a[i] ^ a[i + 1]);

    int m = n - 2; // number of valid start positions (0-indexed)

    // per position: 6 permutations, store all needed quantities
    vector<array<LL, 6>> total_delta(m);
    vector<array<LL, 6>> delta_left(m);
    vector<array<LL, 6>> delta_right(m);
    vector<array<int, 6>> left_val(m);
    vector<array<int, 6>> right_val(m);
    vector<LL> best(m, 0);

    // the 6 permutations of 3 indices
    const int perms[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

    for (int i = 0; i < m; ++i) {
        int x = a[i], y = a[i + 1], z = a[i + 2];
        int vals[3] = {x, y, z};

        for (int p = 0; p < 6; ++p) {
            int b0 = vals[perms[p][0]];
            int b1 = vals[perms[p][1]];
            int b2 = vals[perms[p][2]];

            left_val[i][p] = b0;
            right_val[i][p] = b2;

            LL dl = 0;
            if (i > 0) dl = (LL)(a[i - 1] ^ b0) - (a[i - 1] ^ x);
            delta_left[i][p] = dl;

            LL din = (LL)(b0 ^ b1) + (b1 ^ b2) - (x ^ y) - (y ^ z);

            LL dr = 0;
            if (i < n - 3) dr = (LL)(b2 ^ a[i + 3]) - (z ^ a[i + 3]);
            delta_right[i][p] = dr;

            total_delta[i][p] = dl + din + dr;
            if (total_delta[i][p] > best[i]) best[i] = total_delta[i][p];
        }
    }

    LL ans = base;

    // Case 1: |i - j| >= 4  (independent blocks)
    vector<LL> pref_max(m), suff_max(m);
    pref_max[0] = best[0];
    for (int i = 1; i < m; ++i) pref_max[i] = max(pref_max[i - 1], best[i]);
    suff_max[m - 1] = best[m - 1];
    for (int i = m - 2; i >= 0; --i) suff_max[i] = max(suff_max[i + 1], best[i]);

    for (int i = 0; i < m; ++i) {
        if (i >= 4) ans = max(ans, base + best[i] + pref_max[i - 4]);
        if (i + 4 < m) ans = max(ans, base + best[i] + suff_max[i + 4]);
    }

    // Case 2: |i - j| = 3  (shared boundary edge)
    for (int i = 0; i + 3 < m; ++i) {
        int j = i + 3;
        LL shared_orig = (LL)(a[i + 2] ^ a[j]);
        for (int p = 0; p < 6; ++p) {
            for (int q = 0; q < 6; ++q) {
                LL combined = base + total_delta[i][p] + total_delta[j][q] - delta_right[i][p] - delta_left[j][q] +
                              (LL)(right_val[i][p] ^ left_val[j][q]) - shared_orig;
                if (combined > ans) ans = combined;
            }
        }
    }

    cout << ans << '\n';
    return 0;
}
