#include <iostream>
using namespace std;

int main() {
    // A = 99...9 (20260606 nines), B = 99...9 (2026 nines)
    // S = A * B = (10^{20260606} - 1)(10^{2026} - 1)
    //   = 10^{20262632} - 10^{20260606} - 10^{2026} + 1
    //
    // Digit pattern of S (positions from left, 1-indexed):
    //   1 .. m-1:      9
    //   m:             8
    //   m+1 .. n:      9
    //   n+1:           0
    //   n+2 .. n+m-1:  0
    //   n+m:           1
    // where n = 20260606, m = 2026.
    //
    // d1 = position 2026 = m          -> 8
    // d2 = position 20260523          -> falls in group m+1..n (9's) -> 9
    // d3 = position 20262632 = n+m    -> 1
    cout << 891 << endl;
    return 0;
}
