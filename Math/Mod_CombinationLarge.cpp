// 巨大な N (N <= 10^9 程度, N < MOD) に対する階乗・組合せを O(√MOD log MOD) で計算するライブラリ
//
// example:
//
// yosupo judge - Many Factorials
// https://judge.yosupo.jp/submission/391645
// 巨大階乗計算
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T, ll primitive_root = 3>
struct Mod_CombinationLarge {
private:
    ll v;
    vector<T> fact_table, invFact;
    mutable vector<ll> rev;

    // NTT (Number Theoretic Transform)
    void ntt(vector<T>& a, bool invert) const {
        ll n = a.size();
        ll mod = T(0).mod();
        if ((ll)rev.size() != n) {
            rev.assign(n, 0);
            for (ll i = 0; i < n; i++) {
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
            }
        }
        for (ll i = 0; i < n; i++) {
            if (i < rev[i]) swap(a[i], a[rev[i]]);
        }
        for (ll len = 2; len <= n; len <<= 1) {
            T wlen = T(primitive_root).pow((mod - 1) / len);
            if (invert) wlen = wlen.inv();
            for (ll i = 0; i < n; i += len) {
                T w = 1;
                for (ll j = 0; j < len / 2; j++) {
                    T u = a[i + j];
                    T v_val = a[i + j + len / 2] * w;
                    a[i + j] = u + v_val;
                    a[i + j + len / 2] = u - v_val;
                    w *= wlen;
                }
            }
        }
        if (invert) {
            T n_inv = T(n).inv();
            for (T& x : a) x *= n_inv;
        }
    }

    // 多項式の乗算
    vector<T> convolution(vector<T> a, vector<T> b) const {
        ll n = 1, sz = a.size() + b.size() - 1;
        while (n < sz) n <<= 1;
        a.resize(n, T(0)); b.resize(n, T(0));
        ntt(a, false); ntt(b, false);
        for (ll i = 0; i < n; i++) a[i] *= b[i];
        ntt(a, true);
        a.resize(sz);
        return a;
    }

    // 多項式のシフト
    vector<T> shift(const vector<T>& g, T m) const {
        ll d = g.size() - 1;
        vector<T> res(d + 1);
        
        vector<T> A(d + 1), B(2 * d + 1);
        for (ll i = 0; i <= d; i++) {
            A[i] = g[i] * invFact[i] * invFact[d - i];
            if ((d - i) % 2 != 0) A[i] = -A[i];
        }
        for (ll i = 0; i <= 2 * d; i++) {
            T val = m + T(i - d);
            if (val.val() == 0) B[i] = 1; 
            else B[i] = val.inv();
        }
        
        vector<T> C = convolution(A, B);
        
        T prod = 1;
        for (ll j = 0; j <= d; j++) {
            prod *= (m - T(j));
        }
        
        for (ll k = 0; k <= d; k++) {
            ll val_idx = (m + T(k)).val();
            if (val_idx <= d) {
                res[k] = g[val_idx]; 
            } else {
                res[k] = C[k + d] * prod;
            }
            
            T den = m + T(k - d);
            if (den.val() == 0) {
                prod = 1;
                for (ll j = 0; j <= d; j++) {
                    if (j != d) prod *= (m + T(k + 1 - j));
                }
            } else {
                prod = prod * (m + T(k + 1)) * den.inv();
            }
        }
        return res;
    }

public:
    // コンストラクタ (O(√MOD log MOD) の前計算を実行)
    Mod_CombinationLarge() {
        ll mod = T(0).mod();
        v = 1;
        while (v * v < mod) v *= 2;
        
        invFact.assign(v + 1, T(0));
        invFact[0] = 1;
        T fact_v = 1;
        for (ll i = 1; i <= v; i++) fact_v *= T(i);
        invFact[v] = fact_v.inv();
        for (ll i = v - 1; i >= 1; i--) invFact[i] = invFact[i + 1] * T(i + 1);
        
        vector<T> g = {T(1), T(v + 1)};
        ll d = 1;
        while (d < v) {
            vector<T> g1 = shift(g, T(d + 1));
            vector<T> g2 = shift(g, T(d) * T(v).inv());
            vector<T> g3 = shift(g, T(d) * T(v).inv() + T(d + 1));
            
            vector<T> P_all(2 * d + 1);
            vector<T> P_shift_all(2 * d + 1);
            
            for (ll i = 0; i <= d; i++) P_all[i] = g[i];
            for (ll i = d + 1; i <= 2 * d; i++) P_all[i] = g1[i - d - 1];
            
            for (ll i = 0; i <= d; i++) P_shift_all[i] = g2[i];
            for (ll i = d + 1; i <= 2 * d; i++) P_shift_all[i] = g3[i - d - 1];
            
            vector<T> next_g(2 * d + 1);
            for (ll i = 0; i <= 2 * d; i++) {
                next_g[i] = P_all[i] * P_shift_all[i];
            }
            g = next_g;
            d *= 2;
        }
        
        fact_table.assign(v + 1, T(1));
        for (ll i = 0; i < v; i++) {
            fact_table[i + 1] = fact_table[i] * g[i];
        }
    }

    // 階乗 n!
    T fact(ll n) const {
        ll mod = T(0).mod();
        if (n >= mod) return 0;
        ll q = n / v, r = n % v;
        
        if (r <= v / 2) {
            T res = fact_table[q];
            T val = T(q * v + 1);
            while (r > 0) {
                res *= val;
                val += T(1);
                r--;
            }
            return res;
        } else {
            T num = fact_table[q + 1];
            T den = 1;
            T val = T(n + 1);
            ll rem = v - r;
            while (rem > 0) {
                den *= val;
                val += T(1);
                rem--;
            }
            return num * den.inv();
        }
    }

    // 階乗の逆元 (n!)^-1
    T inv_fact(ll n) const {
        ll mod = T(0).mod();
        if (n >= mod) return 0;
        return fact(n).inv();
    }

    // 組合せ nCr
    T nCr(ll n, ll r) const {
        if (n < r || r < 0) return 0;
        return fact(n) * inv_fact(r) * inv_fact(n - r);
    }

    // 順列 nPr
    T nPr(ll n, ll r) const {
        if (n < r || r < 0) return 0;
        return fact(n) * inv_fact(n - r);
    }

    // 重複組合せ nHr
    T nHr(ll n, ll r) const {
        if (n == 0 && r == 0) return 1;
        if (n == 0 || r < 0) return 0;
        return nCr(n + r - 1, r);
    }

    // 多項係数
    T multinomial(const vector<ll>& r) const {
        ll sum = 0;
        for (ll x : r) {
            if (x < 0) return 0;
            sum += x;
        }
        T res = fact(sum);
        for (ll x : r) res *= inv_fact(x);
        return res;
    }
};