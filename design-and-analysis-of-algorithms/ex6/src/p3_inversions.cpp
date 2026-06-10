#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

ll merge_and_count(vector<int> &a, int l, int mid, int r, vector<int> &tmp) {
    int i = l, j = mid, k = l;
    ll inv = 0;
    while (i < mid && j < r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            inv += mid - i;
            tmp[k++] = a[j++];
        }
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int p = l; p < r; ++p) a[p] = tmp[p];
    return inv;
}

ll count_inversions(vector<int> &a, int l, int r, vector<int> &tmp) {
    if (r - l <= 1) return 0;
    int mid = l + (r - l) / 2;
    ll inv = count_inversions(a, l, mid, tmp);
    inv += count_inversions(a, mid, r, tmp);
    inv += merge_and_count(a, l, mid, r, tmp);
    return inv;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x : a) cin >> x;

    vector<int> tmp(n);
    cout << count_inversions(a, 0, n, tmp) << '\n';
}
