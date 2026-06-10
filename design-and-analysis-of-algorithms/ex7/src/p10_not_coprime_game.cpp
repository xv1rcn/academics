#include <bitset>
#include <iostream>
#include <vector>
using namespace std;

const int MAXA = 300000;
const int MAX_G = MAXA / 2 + 5; // max possible Grundy value (for even MAXA: 149999)
const int SMALL_P = 1000;       // threshold for bitset storage

int spf[MAXA + 1];
vector<int> primes;
int G[MAXA + 1];

// Small odd primes (<= SMALL_P) indexed by id
vector<int> small_primes;
int small_id[MAXA + 1]; // -1 if not small odd prime, else index in small_primes

// reachable[p_id] = bitset of Grundy values from multiples of this prime processed so far
vector<bitset<MAX_G>> reachable_small;

// Large odd primes (> SMALL_P): for each, store (multiple, Grundy) pairs
struct LargeEntry {
    int multiple;
    int grundy;
};

vector<vector<LargeEntry>> reachable_large;
int large_id[MAXA + 1]; // -1 if not large odd prime, else index in reachable_large

void init_grundy() {
    // Linear sieve for spf and primes
    fill(spf, spf + MAXA + 1, 0);
    for (int i = 2; i <= MAXA; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > spf[i] || (long long)i * p > MAXA) break;
            spf[i * p] = p;
        }
    }

    // Classify odd primes into small and large
    fill(small_id, small_id + MAXA + 1, -1);
    fill(large_id, large_id + MAXA + 1, -1);

    for (int p : primes) {
        if (p == 2) continue;
        if (p <= SMALL_P) {
            small_id[p] = (int)small_primes.size();
            small_primes.push_back(p);
        } else {
            large_id[p] = (int)reachable_large.size();
            reachable_large.push_back({});
        }
    }

    reachable_small.assign(small_primes.size(), bitset<MAX_G>());

    // G[1] = 0
    G[1] = 0;

    // Local bitset for union during mex computation (odd composites with spf >= 5)
    bitset<MAX_G> seen;

    // Process numbers 2..MAXA
    for (int x = 2; x <= MAXA; x++) {
        // --- Compute G[x] ---
        if (x % 2 == 0) {
            G[x] = x / 2 - 1;
        } else if (spf[x] == x) {
            // odd prime
            G[x] = 0;
        } else if (spf[x] == 3) {
            // odd composite, divisible by 3
            G[x] = (x - 3) / 4;
        } else {
            // odd composite, spf >= 5
            // Collect distinct odd prime factors
            int tmp = x;
            vector<int> odd_factors;
            while (tmp > 1) {
                int p = spf[tmp];
                if (p != 2) {
                    if (odd_factors.empty() || odd_factors.back() != p) odd_factors.push_back(p);
                }
                tmp /= p;
            }

            seen.reset();
            for (int p : odd_factors) {
                int sid = small_id[p];
                if (sid != -1) {
                    seen |= reachable_small[sid];
                } else {
                    int lid = large_id[p];
                    for (auto &entry : reachable_large[lid]) {
                        if (entry.multiple < x) {
                            seen.set(entry.grundy);
                        } else {
                            break;
                        }
                    }
                }
            }

            // Find mex: first zero bit
            int g = 0;
            while (seen[g]) g++;
            G[x] = g;
        }

        // --- Update reachable sets for odd prime factors ---
        int tmp = x;
        int last_p = 0;
        while (tmp > 1) {
            int p = spf[tmp];
            if (p != 2 && p != last_p) {
                int sid = small_id[p];
                if (sid != -1) {
                    reachable_small[sid].set(G[x]);
                } else {
                    int lid = large_id[p];
                    reachable_large[lid].push_back({x, G[x]});
                }
                last_p = p;
            }
            tmp /= p;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    init_grundy();

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        int xor_sum = 0;
        for (int i = 0; i < n; i++) {
            int a;
            cin >> a;
            xor_sum ^= G[a];
        }
        cout << (xor_sum != 0 ? "Yes" : "No") << "\n";
    }
    return 0;
}
