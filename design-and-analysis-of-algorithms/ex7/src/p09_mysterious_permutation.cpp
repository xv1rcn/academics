#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1000000007;

int modpow(long long a, int e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return (int)res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int k = (n + 1) / 2; // ceil(n/2)

    // Compute n! mod MOD
    long long fact_n = 1;
    for (int i = 2; i <= n; ++i) { fact_n = fact_n * i % MOD; }

    // Precompute inverse factorials inv_fact[i] = 1/i! mod MOD
    vector<int> inv_fact(n + 1);
    inv_fact[n] = modpow(fact_n, MOD - 2);
    for (int i = n - 1; i >= 0; --i) { inv_fact[i] = (int)((long long)inv_fact[i + 1] * (i + 1) % MOD); }

    // Precompute F[t] = sum_{i=0}^{t} (-1)^i / i!
    int limit = n - k;
    vector<int> F(limit + 1);
    F[0] = 1;
    for (int t = 1; t <= limit; ++t) {
        long long term = inv_fact[t];
        if (t & 1) term = MOD - term; // (-1)^t for odd t
        F[t] = (int)((F[t - 1] + term) % MOD);
    }

    // Sum over m from k to n: term = n! * inv_fact[m] * F[n-m]
    long long total = 0;
    for (int m = k; m <= n; ++m) { total = (total + fact_n * inv_fact[m] % MOD * F[n - m]) % MOD; }

    cout << total << "\n";
    return 0;
}
