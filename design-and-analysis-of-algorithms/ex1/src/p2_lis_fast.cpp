#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    vector<int> tails;
    tails.reserve(n);

    for (int x : a) {
        auto it = ranges::lower_bound(tails, x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    cout << tails.size() << '\n';
}
