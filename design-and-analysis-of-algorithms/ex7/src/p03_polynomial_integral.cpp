#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
using namespace std;

using ll = long long;

struct Term {
    ll num; // numerator (may be negative; sign belongs here)
    ll den; // denominator (always positive)
    ll deg; // degree after integration
};

ll gcd(ll a, ll b) {
    a = abs(a);
    b = abs(b);
    while (b) {
        ll t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Parse the polynomial string into (coefficient, degree) pairs
vector<pair<ll, ll>> parse(const string &s) {
    vector<pair<ll, ll>> terms;
    int n = (int)s.size();
    int i = 0;

    while (i < n) {
        ll sign = 1;
        if (s[i] == '+') {
            sign = 1;
            ++i;
        } else if (s[i] == '-') {
            sign = -1;
            ++i;
        }

        // Parse coefficient digits
        ll coeff = 0;
        bool have_coeff = false;
        while (i < n && isdigit(s[i])) {
            coeff = coeff * 10 + (s[i] - '0');
            have_coeff = true;
            ++i;
        }
        if (!have_coeff) coeff = 1;
        coeff *= sign;

        // Parse x and degree
        ll degree = 0;
        if (i < n && s[i] == 'x') {
            ++i;
            if (i < n && s[i] == '^') {
                ++i;
                degree = 0;
                while (i < n && isdigit(s[i])) {
                    degree = degree * 10 + (s[i] - '0');
                    ++i;
                }
            } else {
                degree = 1;
            }
        } else {
            degree = 0;
        }

        terms.emplace_back(coeff, degree);
    }

    return terms;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    auto raw = parse(s);

    vector<Term> result;
    for (auto [coeff, deg] : raw) {
        ll num = coeff;
        ll den = deg + 1;
        ll new_deg = deg + 1;

        ll g = gcd(num, den);
        num /= g;
        den /= g;

        // Ensure denominator is positive
        if (den < 0) {
            num = -num;
            den = -den;
        }

        result.push_back({num, den, new_deg});
    }

    // All terms are already sorted by degree descending (parsed left-to-right)
    for (size_t idx = 0; idx < result.size(); ++idx) {
        auto [num, den, deg] = result[idx];

        // Determine sign prefix
        if (idx == 0) {
            if (num < 0) cout << '-';
        } else {
            cout << (num > 0 ? '+' : '-');
        }

        ll abs_num = abs(num);

        // Coefficient part
        if (den == 1) {
            // Integer coefficient
            if (abs_num != 1) { cout << abs_num; }
            // if abs_num == 1, omit the 1
        } else {
            // Fractional coefficient
            // The rule says to omit 1 only if the ENTIRE coefficient is ±1.
            // For a fraction, that would require den == 1 (already handled above).
            cout << abs_num << '/' << den;
        }

        // x part
        if (deg == 1) {
            cout << 'x';
        } else {
            cout << "x^" << deg;
        }
    }

    cout << '\n';
    return 0;
}
