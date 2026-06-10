#include <iostream>
#include <vector>

using namespace std;

const long long MOD = 998244353;

class SegTree {
    int n;
    vector<long long> sum, add, mul;

    void apply(int u, int l, int r, long long m, long long a) {
        sum[u] = (sum[u] * m + a * (r - l + 1)) % MOD;
        mul[u] = mul[u] * m % MOD;
        add[u] = (add[u] * m + a) % MOD;
    }

    void push(int u, int l, int r) {
        if (mul[u] == 1 && add[u] == 0) return;
        int mid = (l + r) >> 1;
        long long m = mul[u], a = add[u];
        apply(u * 2, l, mid, m, a);
        apply(u * 2 + 1, mid + 1, r, m, a);
        mul[u] = 1;
        add[u] = 0;
    }

    void build(int u, int l, int r, const vector<int> &a) {
        mul[u] = 1;
        add[u] = 0;
        if (l == r) {
            sum[u] = a[l - 1] % MOD;
            return;
        }
        int mid = (l + r) >> 1;
        build(u * 2, l, mid, a);
        build(u * 2 + 1, mid + 1, r, a);
        sum[u] = (sum[u * 2] + sum[u * 2 + 1]) % MOD;
    }

    void range_add(int u, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) {
            apply(u, l, r, 1, v);
            return;
        }
        push(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) range_add(u * 2, l, mid, ql, qr, v);
        if (qr > mid) range_add(u * 2 + 1, mid + 1, r, ql, qr, v);
        sum[u] = (sum[u * 2] + sum[u * 2 + 1]) % MOD;
    }

    void range_mul(int u, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) {
            apply(u, l, r, v, 0);
            return;
        }
        push(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) range_mul(u * 2, l, mid, ql, qr, v);
        if (qr > mid) range_mul(u * 2 + 1, mid + 1, r, ql, qr, v);
        sum[u] = (sum[u * 2] + sum[u * 2 + 1]) % MOD;
    }

    long long query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return sum[u];
        push(u, l, r);
        int mid = (l + r) >> 1;
        long long res = 0;
        if (ql <= mid) res += query(u * 2, l, mid, ql, qr);
        if (qr > mid) res += query(u * 2 + 1, mid + 1, r, ql, qr);
        return res % MOD;
    }

public:
    SegTree(int n, const vector<int> &a)
        : n(n)
        , sum(4 * n)
        , add(4 * n)
        , mul(4 * n) {
        build(1, 1, n, a);
    }

    void range_add(int l, int r, long long v) {
        range_add(1, 1, n, l, r, v);
    }

    void range_mul(int l, int r, long long v) {
        range_mul(1, 1, n, l, r, v % MOD);
    }

    long long range_sum(int l, int r) {
        return query(1, 1, n, l, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    SegTree seg(n, a);

    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int l, r;
            long long k;
            cin >> l >> r >> k;
            seg.range_add(l, r, k);
        } else if (op == 2) {
            int l, r;
            cin >> l >> r;
            cout << seg.range_sum(l, r) << '\n';
        } else if (op == 3) {
            int l, r;
            long long k;
            cin >> l >> r >> k;
            seg.range_mul(l, r, k);
        }
    }

    return 0;
}
