#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

using u32 = unsigned;
using u64 = unsigned long long;

constexpr u32 K = 26;
constexpr u32 S = K * K; // 676
constexpr u32 MOD = 998244353;

inline u32 idx(u32 x, u32 y) {
    return x * K + y;
}

u64 mod_pow(u64 a, u64 e) {
    u64 res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

u64 mod_inv(u64 a) {
    return mod_pow(a, MOD - 2);
}

void transpose(const vector<u32> &B, vector<u32> &B_T) {
    for (u32 k = 0; k < S; k++) {
        u32 kS = k * S;
        for (u32 j = 0; j < S; j++) { B_T[j * S + k] = B[kS + j]; }
    }
}

// C = A * B  using B supplied as its transpose B_T.
// Uses unsigned __int128 for sum accumulation, safe for up to ~1.8e19.
void mat_mul_T(const vector<u32> &A, const vector<u32> &B_T, vector<u32> &C) {
    for (u32 i = 0; i < S; i++) {
        u32 iS = i * S;
        const u32 *__restrict Arow = &A[iS];
        u32 *__restrict Crow = &C[iS];
        for (u32 j = 0; j < S; j++) {
            const u32 *__restrict BTrow = &B_T[j * S];
            unsigned __int128 sum = 0;
#pragma clang loop vectorize(enable)
            for (u32 k = 0; k < S; k++) { sum += (u64)Arow[k] * BTrow[k]; }
            Crow[j] = (u32)(sum % MOD);
        }
    }
}

u64 mat_trace(const vector<u32> &M) {
    u64 sum = 0;
    for (u32 i = 0; i < S; i++) sum = (sum + M[i * S + i]) % MOD;
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ---- Build M and M_neg ----
    vector<u32> M(S * S, 0);
    vector<u32> M_neg(S * S, 0);

    for (u32 x = 0; x < K; x++) {
        for (u32 y = 0; y < K; y++) {
            u32 from = idx(x, y);
            u32 fromS = from * S;
            for (u32 z = 0; z < K; z++) {
                if (x + y + z == 26) continue;
                u32 to = idx(y, z);
                M[fromS + to] = 1;
                M_neg[fromS + to] = (y == 2 && z == 6) ? MOD - 1 : 1;
            }
        }
    }

    // Transpose
    vector<u32> M_T(S * S), M_neg_T(S * S);
    transpose(M, M_T);
    transpose(M_neg, M_neg_T);

    // ---- d = 1 ----
    u64 A1 = 0;
    for (u32 x = 0; x < K; x++) { A1 = (A1 + M[idx(x, x) * S + idx(x, x)]) % MOD; }
    // h1 = A1 (2026/1 = 2026 even)

    // ---- d = 2 ----
    vector<u32> M2(S * S), M2_T(S * S);
    mat_mul_T(M, M_T, M2);
    transpose(M2, M2_T);
    u64 A2 = mat_trace(M2);

    vector<u32> Mneg2(S * S), Mneg2_T(S * S);
    mat_mul_T(M_neg, M_neg_T, Mneg2);
    transpose(Mneg2, Mneg2_T);
    u64 B2 = mat_trace(Mneg2);

    u64 inv2 = (MOD + 1) / 2;
    u64 C2_0 = (A2 + B2) % MOD * inv2 % MOD;
    // h2 = C2_0 (2026/2 = 1013 odd)

    // ---- Precompute M^{2^k} for k = 0..9 ----
    vector<vector<u32>> M_pow(10), M_pow_T(10);
    M_pow[0] = M;
    M_pow_T[0] = M_T;
    M_pow[1] = M2;
    M_pow_T[1] = M2_T;
    for (int k = 2; k <= 9; k++) {
        M_pow[k].resize(S * S);
        mat_mul_T(M_pow[k - 1], M_pow_T[k - 1], M_pow[k]);
        M_pow_T[k].resize(S * S);
        transpose(M_pow[k], M_pow_T[k]);
    }

    // ---- Compute M^1013 by combining precomputed powers ----
    // 1013 = 512+256+128+64+32+16+4+1 -> bits 0,2,4,5,6,7,8,9
    // Start from M^1 (bit 0) and multiply by bits 2,4,5,6,7,8,9
    vector<u32> M1013(S * S), M1013_T(S * S);
    M1013 = M_pow[0];
    M1013_T = M_pow_T[0];
    vector<u32> tmp_mat(S * S);
    int mbits[] = {2, 4, 5, 6, 7, 8, 9};
    for (int b : mbits) {
        fill(tmp_mat.begin(), tmp_mat.end(), 0);
        mat_mul_T(M1013, M_pow_T[b], tmp_mat);
        swap(M1013, tmp_mat);
        transpose(M1013, M1013_T);
    }

    u64 A1013 = mat_trace(M1013);
    // h1013 = A1013 (2026/1013 = 2 even)

    // ---- tr(M^2026) from M^1013 without full matrix multiply ----
    u64 A2026 = 0;
    for (u32 i = 0; i < S; i++) {
        u32 iS = i * S;
        for (u32 j = 0; j < S; j++) { A2026 = (A2026 + (u64)M1013[iS + j] * M1013[j * S + i]) % MOD; }
    }

    // ---- Precompute M_neg^{2^k} for k = 0..10 ----
    vector<vector<u32>> Mneg_pow(11), Mneg_pow_T(11);
    Mneg_pow[0] = M_neg;
    Mneg_pow_T[0] = M_neg_T;
    Mneg_pow[1] = Mneg2;
    Mneg_pow_T[1] = Mneg2_T;
    for (int k = 2; k <= 10; k++) {
        Mneg_pow[k].resize(S * S);
        mat_mul_T(Mneg_pow[k - 1], Mneg_pow_T[k - 1], Mneg_pow[k]);
        Mneg_pow_T[k].resize(S * S);
        transpose(Mneg_pow[k], Mneg_pow_T[k]);
    }

    // ---- Compute M_neg^2026 by combining precomputed powers ----
    // 2026 = 1024+512+256+128+64+32+8+2 -> bits 1,3,5,6,7,8,9,10
    // Start from M_neg^2 (bit 1) and multiply by bits 3,5,6,7,8,9,10
    vector<u32> Mneg2026(S * S), Mneg2026_T(S * S);
    Mneg2026 = Mneg_pow[1];
    Mneg2026_T = Mneg_pow_T[1];
    int negbits[] = {3, 5, 6, 7, 8, 9, 10};
    for (int b : negbits) {
        fill(tmp_mat.begin(), tmp_mat.end(), 0);
        mat_mul_T(Mneg2026, Mneg_pow_T[b], tmp_mat);
        swap(Mneg2026, tmp_mat);
        transpose(Mneg2026, Mneg2026_T);
    }

    u64 B2026 = mat_trace(Mneg2026);

    u64 C2026_0 = (A2026 + B2026) % MOD * inv2 % MOD;
    // h2026 = C2026_0 (2026/2026 = 1 odd)

    // ---- Burnside ----
    u64 h1 = A1;         // 2026/1 = 2026 even
    u64 h2 = C2_0;       // 2026/2 = 1013 odd
    u64 h1013 = A1013;   // 2026/1013 = 2 even
    u64 h2026 = C2026_0; // 2026/2026 = 1 odd

    // phi values
    u64 phi_2026 = 1012; // φ(2026)
    u64 phi_1013 = 1012; // φ(1013)
    u64 phi_2 = 1;       // φ(2)
    u64 phi_1 = 1;       // φ(1)

    u64 total = (phi_2026 * h1 + phi_1013 * h2 + phi_2 * h1013 + phi_1 * h2026) % MOD;
    total = total * mod_inv(2026) % MOD;

    cout << total << "\n";
    return 0;
}
