// 巨大なN (N <= MOD) に対する階乗・組合せを高速計算するライブラリ
//
// example:
//
// yosupo judge - Many Factorials
// https://judge.yosupo.jp/submission/391271
// 高速階乗計算
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template<ll MOD = 998244353>
struct CombinationLarge {
    ll v;
    vector<ll> fact_table, invFact;
    mutable vector<int> rev; // NTT用ビット反転テーブル

    // 累乗計算
    ll modpow(ll a, ll b) const {
        ll res = 1;
        a %= MOD;
        while (b > 0) {
            if (b & 1) res = res * a % MOD;
            a = a * a % MOD;
            b >>= 1;
        }
        return res;
    }

    // NTT (Number Theoretic Transform) - 定数倍高速化版
    void ntt(vector<ll>& a, bool invert) const {
        int n = a.size();
        if ((int)rev.size() != n) {
            rev.assign(n, 0);
            for (int i = 0; i < n; i++) {
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
            }
        }
        for (int i = 0; i < n; i++) {
            if (i < rev[i]) swap(a[i], a[rev[i]]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = modpow(3, (MOD - 1) / len);
            if (invert) wlen = modpow(wlen, MOD - 2);
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j];
                    ll v_val = a[i + j + len / 2] * w % MOD;
                    a[i + j] = u + v_val;
                    if (a[i + j] >= MOD) a[i + j] -= MOD;
                    a[i + j + len / 2] = u - v_val;
                    if (a[i + j + len / 2] < 0) a[i + j + len / 2] += MOD;
                    w = w * wlen % MOD;
                }
            }
        }
        if (invert) {
            ll n_inv = modpow(n, MOD - 2);
            for (ll& x : a) x = x * n_inv % MOD;
        }
    }

    // 多項式の乗算
    vector<ll> convolution(vector<ll> a, vector<ll> b) const {
        ll n = 1, sz = a.size() + b.size() - 1;
        while (n < sz) n <<= 1;
        a.resize(n); b.resize(n);
        ntt(a, false); ntt(b, false);
        for (ll i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
        ntt(a, true);
        a.resize(sz);
        return a;
    }

    // 多項式のシフト
    vector<ll> shift(const vector<ll>& g, ll m) const {
        ll d = g.size() - 1;
        vector<ll> res(d + 1);
        m = (m % MOD + MOD) % MOD;
        
        vector<ll> A(d + 1), B(2 * d + 1);
        for (ll i = 0; i <= d; i++) {
            A[i] = g[i] * invFact[i] % MOD * invFact[d - i] % MOD;
            if ((d - i) % 2 != 0) A[i] = (MOD - A[i]) % MOD;
        }
        for (ll i = 0; i <= 2 * d; i++) {
            ll val = (m + i - d) % MOD;
            if (val < 0) val += MOD;
            if (val == 0) B[i] = 1; 
            else B[i] = modpow(val, MOD - 2);
        }
        
        vector<ll> C = convolution(A, B);
        
        ll prod = 1;
        for (ll j = 0; j <= d; j++) {
            ll v_val = (m - j) % MOD;
            if (v_val < 0) v_val += MOD;
            prod = prod * v_val % MOD;
        }
        
        for (ll k = 0; k <= d; k++) {
            ll val = (m + k) % MOD;
            if (val <= d) {
                res[k] = g[val]; 
            } else {
                res[k] = C[k + d] * prod % MOD;
            }
            
            ll num = (m + k + 1) % MOD;
            ll den = (m + k - d) % MOD;
            if (den < 0) den += MOD;
            
            if (den == 0) {
                prod = 1;
                for (ll j = 0; j <= d; j++) {
                    ll v_val = (m + k + 1 - j) % MOD;
                    if (v_val < 0) v_val += MOD;
                    prod = prod * v_val % MOD;
                }
            } else {
                prod = prod * num % MOD * modpow(den, MOD - 2) % MOD;
            }
        }
        return res;
    }

    // 前処理
    void build() {
        v = 1;
        while (v * v < MOD) v *= 2;
        
        invFact.assign(v + 1, 0);
        invFact[0] = 1;
        ll fact_v = 1;
        for (ll i = 1; i <= v; i++) fact_v = fact_v * i % MOD;
        invFact[v] = modpow(fact_v, MOD - 2);
        for (ll i = v - 1; i >= 1; i--) invFact[i] = invFact[i + 1] * (i + 1) % MOD;
        
        vector<ll> g = {1, v + 1};
        ll d = 1;
        while (d < v) {
            vector<ll> g1 = shift(g, d + 1);
            vector<ll> g2 = shift(g, d * modpow(v, MOD - 2) % MOD);
            vector<ll> g3 = shift(g, (d * modpow(v, MOD - 2) + d + 1) % MOD);
            
            vector<ll> P_all(2 * d + 1);
            vector<ll> P_shift_all(2 * d + 1);
            
            for (ll i = 0; i <= d; i++) P_all[i] = g[i];
            for (ll i = d + 1; i <= 2 * d; i++) P_all[i] = g1[i - d - 1];
            
            for (ll i = 0; i <= d; i++) P_shift_all[i] = g2[i];
            for (ll i = d + 1; i <= 2 * d; i++) P_shift_all[i] = g3[i - d - 1];
            
            vector<ll> next_g(2 * d + 1);
            for (ll i = 0; i <= 2 * d; i++) {
                next_g[i] = P_all[i] * P_shift_all[i] % MOD;
            }
            g = next_g;
            d *= 2;
        }
        
        fact_table.assign(v + 1, 1);
        for (ll i = 0; i < v; i++) {
            fact_table[i + 1] = fact_table[i] * g[i] % MOD;
        }
    }

    CombinationLarge() { build(); }

    // n! mod MOD
    ll fact(ll n) const {
        if (n >= MOD) return 0;
        ll q = n / v, r = n % v;
        
        if (r <= v / 2) {
            ll res = fact_table[q];
            ll val = q * v + 1;
            while (r >= 8) {
                ll p1 = val * (val + 1) % MOD;
                ll p2 = (val + 2) * (val + 3) % MOD;
                ll p3 = (val + 4) * (val + 5) % MOD;
                ll p4 = (val + 6) * (val + 7) % MOD;
                res = res * p1 % MOD * p2 % MOD * p3 % MOD * p4 % MOD;
                val += 8;
                r -= 8;
            }
            while (r > 0) {
                res = res * val % MOD;
                val++;
                r--;
            }
            return res;
        } else {
            ll num = fact_table[q + 1];
            ll den = 1;
            ll val = n + 1;
            ll rem = v - r;
            while (rem >= 8) {
                ll p1 = val * (val + 1) % MOD;
                ll p2 = (val + 2) * (val + 3) % MOD;
                ll p3 = (val + 4) * (val + 5) % MOD;
                ll p4 = (val + 6) * (val + 7) % MOD;
                den = den * p1 % MOD * p2 % MOD * p3 % MOD * p4 % MOD;
                val += 8;
                rem -= 8;
            }
            while (rem > 0) {
                den = den * val % MOD;
                val++;
                rem--;
            }
            return num * modpow(den, MOD - 2) % MOD;
        }
    }

    // (n!)^-1 mod MOD
    ll inv_fact(ll n) const {
        if (n >= MOD) return 0;
        return modpow(fact(n), MOD - 2);
    }

    // nPr mod MOD
    ll nPr(ll n, ll r) const {
        if (n < r || r < 0) return 0;
        return fact(n) * inv_fact(n - r) % MOD;
    }

    // nCr mod MOD
    ll nCr(ll n, ll r) const {
        if (n < r || r < 0) return 0;
        return fact(n) * inv_fact(r) % MOD * inv_fact(n - r) % MOD;
    }

    // nHr mod MOD (重複組合せ)
    ll nHr(ll n, ll r) const {
        if (n == 0 && r == 0) return 1;
        if (n == 0 || r < 0) return 0;
        return nCr(n + r - 1, r);
    }

    // 多項係数 mod MOD
    ll multinomial(const vector<ll>& r) const {
        ll sum = 0;
        for (ll x : r) {
            if (x < 0) return 0;
            sum += x;
        }
        ll res = fact(sum);
        for (ll x : r) res = res * inv_fact(x) % MOD;
        return res;
    }
};