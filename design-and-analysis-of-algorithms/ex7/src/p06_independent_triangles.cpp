#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> top, bot;
    for (int i = 0; i < N; ++i) {
        int x, y;
        cin >> x >> y;
        if (y == 1)
            top.push_back(x);
        else
            bot.push_back(x);
    }

    int A = (int)top.size();
    int B = (int)bot.size();

    sort(top.begin(), top.end());
    sort(bot.begin(), bot.end());

    int maxT = min({N / 3, A, B});

    if (maxT == 0) {
        cout << "0 0\n";
        return 0;
    }

    // feasible x range: x is number of triangles with 2 top + 1 bot
    int lo = max(0, 2 * maxT - B);
    int hi = min(maxT, A - maxT);

    // precompute width prefix sums for outer pairing (largest x - smallest x, etc.)
    vector<long long> topW(maxT + 1, 0), botW(maxT + 1, 0);

    for (int k = 1; k <= min(A / 2, maxT); ++k) topW[k] = topW[k - 1] + (top[A - k] - top[k - 1]);

    for (int k = 1; k <= min(B / 2, maxT); ++k) botW[k] = botW[k - 1] + (bot[B - k] - bot[k - 1]);

    long long bestArea = 0;
    for (int x = lo; x <= hi; ++x) {
        int y = maxT - x;
        long long area = topW[x] + botW[y];
        if (area > bestArea) bestArea = area;
    }

    cout << maxT << ' ' << bestArea << '\n';
}
