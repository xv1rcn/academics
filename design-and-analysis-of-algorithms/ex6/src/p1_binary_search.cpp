#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    while (k--) {
        int target;
        cin >> target;
        auto it = ranges::lower_bound(a, target);
        if (it != a.end() && *it == target)
            cout << (it - a.begin() + 1) << '\n';
        else
            cout << "-1\n";
    }
}
