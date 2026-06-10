#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int left, right;
    int cnt;
};

class ChairmanTree {
    vector<Node> tree;
    vector<int> roots;
    int max_val;

    int insert(int prev, int l, int r, int val) {
        int cur = tree.size();
        tree.push_back(tree[prev]);
        tree[cur].cnt++;
        if (l == r) return cur;
        int mid = (l + r) >> 1;
        if (val <= mid) {
            int new_left = insert(tree[prev].left, l, mid, val);
            tree[cur].left = new_left;
        } else {
            int new_right = insert(tree[prev].right, mid + 1, r, val);
            tree[cur].right = new_right;
        }
        return cur;
    }

    int query(int u, int v, int l, int r, int k) {
        if (l == r) return l;
        int mid = (l + r) >> 1;
        int left_cnt = tree[tree[v].left].cnt - tree[tree[u].left].cnt;
        if (left_cnt >= k)
            return query(tree[u].left, tree[v].left, l, mid, k);
        else
            return query(tree[u].right, tree[v].right, mid + 1, r, k - left_cnt);
    }

public:
    ChairmanTree(const vector<int> &a, int max_val)
        : max_val(max_val) {
        tree.push_back({0, 0, 0});
        roots.push_back(0);
        for (int x : a) {
            int new_root = insert(roots.back(), 1, max_val, x);
            roots.push_back(new_root);
        }
    }

    int kth(int l, int r, int k) {
        return query(roots[l - 1], roots[r], 1, max_val, k);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    // 离散化：将原值映射到 [1, m]
    vector<int> vals(a);
    ranges::sort(vals);
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    vector<int> disc(n);
    for (int i = 0; i < n; ++i) disc[i] = ranges::lower_bound(vals, a[i]) - vals.begin() + 1;

    ChairmanTree ct(disc, vals.size());

    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        int idx = ct.kth(l, r, k);
        // 将离散化索引映射回原值
        cout << vals[idx - 1] << '\n';
    }

    return 0;
}
