#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    string t = "^#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    t += '$';

    int n = (int)t.size();
    vector<int> p(n, 0);
    int center = 0, right = 0;

    for (int i = 1; i < n - 1; ++i) {
        int mirror = 2 * center - i;

        if (i < right) p[i] = min(right - i, p[mirror]);

        while (t[i + p[i] + 1] == t[i - p[i] - 1]) ++p[i];

        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }
    }

    int max_len = *max_element(p.begin(), p.end());
    cout << max_len << '\n';
}
