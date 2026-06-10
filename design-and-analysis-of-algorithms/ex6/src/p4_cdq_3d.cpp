#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

using namespace std;

struct Triple {
    int a, b, c, id;
};

struct BIT {
    vector<int> t;
    int n;

    BIT(int sz)
        : t(sz + 2)
        , n(sz) {
    }

    void add(int i, int v) {
        for (; i <= n; i += i & -i) t[i] += v;
    }

    int sum(int i) {
        int s = 0;
        for (; i > 0; i -= i & -i) s += t[i];
        return s;
    }
};

void cdq(vector<Triple> &triples, int l, int r, BIT &bit, vector<int> &ans) {
    if (r - l <= 1) return;
    int mid = l + (r - l) / 2;

    cdq(triples, l, mid, bit, ans);
    cdq(triples, mid, r, bit, ans);

    vector<Triple> merged;
    merged.reserve(r - l);
    int i = l, j = mid;

    while (i < mid && j < r) {
        if (triples[i].b <= triples[j].b) {
            bit.add(triples[i].c, 1);
            merged.push_back(triples[i]);
            ++i;
        } else {
            ans[triples[j].id] += bit.sum(triples[j].c);
            merged.push_back(triples[j]);
            ++j;
        }
    }
    while (i < mid) {
        bit.add(triples[i].c, 1);
        merged.push_back(triples[i]);
        ++i;
    }
    while (j < r) {
        ans[triples[j].id] += bit.sum(triples[j].c);
        merged.push_back(triples[j]);
        ++j;
    }

    for (int p = l; p < mid; ++p) bit.add(triples[p].c, -1);

    for (int p = 0; p < r - l; ++p) triples[l + p] = merged[p];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Triple> triples(n);
    int max_c = 0;
    for (int i = 0; i < n; ++i) {
        cin >> triples[i].a >> triples[i].b >> triples[i].c;
        triples[i].id = i;
        if (triples[i].c > max_c) max_c = triples[i].c;
    }

    ranges::sort(triples, [](const Triple &x, const Triple &y) {
        if (x.a != y.a) return x.a < y.a;
        if (x.b != y.b) return x.b < y.b;
        return x.c < y.c;
    });

    vector<int> c_vals(n);
    for (int i = 0; i < n; ++i) c_vals[i] = triples[i].c;
    ranges::sort(c_vals);
    auto last = ranges::unique(c_vals);
    c_vals.erase(last.begin(), last.end());
    for (auto &t : triples) t.c = ranges::lower_bound(c_vals, t.c) - c_vals.begin() + 1;

    BIT bit(c_vals.size());
    vector<int> ans(n, 1);

    cdq(triples, 0, n, bit, ans);

    for (int x : ans) cout << x << '\n';
}
