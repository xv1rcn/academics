#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int64_t> t(n), w(n);
    vector<int> is_key(n);
    int64_t sum_w = 0;
    for (int i = 0; i < n; ++i) {
        cin >> t[i] >> w[i] >> is_key[i];
        sum_w += w[i];
    }

    int64_t C = sum_w + 1;

    vector<tuple<int64_t, int64_t, int>> tasks;
    tasks.reserve(n);
    for (int i = 0; i < n; ++i) {
        int64_t adj = w[i] + (is_key[i] ? C : int64_t{0});
        tasks.emplace_back(t[i], adj, i);
    }

    sort(tasks.begin(), tasks.end(), [](const auto &a, const auto &b) { return get<0>(a) < get<0>(b); });

    priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>> pq;
    int64_t total_val = 0;
    int core_cnt = 0;

    for (const auto &[deadline, adj, idx] : tasks) {
        pq.emplace(adj, idx);
        total_val += w[idx];
        core_cnt += is_key[idx];

        if (pq.size() > static_cast<size_t>(deadline)) {
            auto [popped_adj, popped_idx] = pq.top();
            pq.pop();
            total_val -= w[popped_idx];
            core_cnt -= is_key[popped_idx];
        }
    }

    cout << core_cnt << ' ' << total_val << '\n';
    return 0;
}
