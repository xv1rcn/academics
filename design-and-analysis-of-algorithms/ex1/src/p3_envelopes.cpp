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
    vector<pair<int, int>> envs(n);
    for (auto &[w, h] : envs) cin >> w >> h;

    // 按宽度升序，同宽时按高度降序
    ranges::sort(envs, [](auto &a, auto &b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second > b.second;
    });

    // 在高度序列上做 LIS
    vector<int> tails;
    tails.reserve(n);
    for (auto &[_, h] : envs) {
        auto it = ranges::lower_bound(tails, h);
        if (it == tails.end())
            tails.push_back(h);
        else
            *it = h;
    }
    cout << tails.size() << '\n';
}
