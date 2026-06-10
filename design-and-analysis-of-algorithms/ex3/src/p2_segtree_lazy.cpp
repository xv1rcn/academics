#include <iostream>
#include <vector>

using namespace std;

class SegTree {
    int n;
    vector<long long> sum, lazy;

    void push(int u, int l, int r) {
        if (lazy[u] == 0) return;
        long long tag = lazy[u];
        int mid = (l + r) >> 1;
        sum[u * 2] += tag * (mid - l + 1);
        lazy[u * 2] += tag;
        sum[u * 2 + 1] += tag * (r - mid);
        lazy[u * 2 + 1] += tag;
        lazy[u] = 0;
    }

    void build(int u, int l, int r, const vector<int> &a) {
        if (l == r) {
            sum[u] = a[l - 1];
            return;
        }
        int mid = (l + r) >> 1;
        build(u * 2, l, mid, a);
        build(u * 2 + 1, mid + 1, r, a);
        sum[u] = sum[u * 2] + sum[u * 2 + 1];
    }

    void add(int u, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) {
            sum[u] += v * (r - l + 1);
            lazy[u] += v;
            return;
        }
        push(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) add(u * 2, l, mid, ql, qr, v);
        if (qr > mid) add(u * 2 + 1, mid + 1, r, ql, qr, v);
        sum[u] = sum[u * 2] + sum[u * 2 + 1];
    }

    long long query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return sum[u];
        push(u, l, r);
        int mid = (l + r) >> 1;
        long long res = 0;
        if (ql <= mid) res += query(u * 2, l, mid, ql, qr);
        if (qr > mid) res += query(u * 2 + 1, mid + 1, r, ql, qr);
        return res;
    }

public:
    SegTree(int n, const vector<int> &a)
        : n(n)
        , sum(4 * n)
        , lazy(4 * n) {
        build(1, 1, n, a);
    }

    void range_add(int l, int r, long long v) {
        add(1, 1, n, l, r, v);
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
        }
    }

    return 0;
}
