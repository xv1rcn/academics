#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll k;
    cin >> n >> k;
    vector<ll> sticks(n);
    ll max_len = 0;
    for (auto &s : sticks) {
        cin >> s;
        max_len = max(max_len, s);
    }

    ll lo = 1, hi = max_len, ans = 0;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        ll cnt = 0;
        for (ll s : sticks) {
            cnt += s / mid;
            if (cnt >= k) break;
        }
        if (cnt >= k) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    cout << ans << '\n';
}
