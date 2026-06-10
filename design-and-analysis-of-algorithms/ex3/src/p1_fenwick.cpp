#include <iostream>
#include <vector>

using namespace std;

class Fenwick {
    vector<long long> bit;
    int n;

public:
    explicit Fenwick(int n)
        : bit(n + 1, 0)
        , n(n) {
    }

    void add(int x, long long v) {
        for (; x <= n; x += x & -x) bit[x] += v;
    }

    long long sum(int x) {
        long long s = 0;
        for (; x > 0; x -= x & -x) s += bit[x];
        return s;
    }

    long long range_sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    Fenwick bit(n);
    for (int i = 1; i <= n; ++i) {
        int x;
        cin >> x;
        bit.add(i, x);
    }

    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int x;
            long long k;
            cin >> x >> k;
            bit.add(x, k);
        } else if (op == 2) {
            int l, r;
            cin >> l >> r;
            cout << bit.range_sum(l, r) << '\n';
        }
    }

    return 0;
}
