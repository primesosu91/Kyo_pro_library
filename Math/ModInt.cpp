// ModInt
//
// example:
//
// ABC 156 D - Bouquet (2Q)
// https://atcoder.jp/contests/abc156/submissions/78186098
// 累乗と二項係数
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <ll MOD>
struct ModInt {
    ll v;
    
    // コンストラクタ (負の数も自動で正規化)
    ModInt(ll x = 0) : v(x % MOD) { 
        if (v < 0) v += MOD; 
    }
    
    // 値を取得
    ll val() const { return v; }
    
    // MODを取得
    ll mod() const { return MOD; }

    // 複合代入演算子
    ModInt& operator+=(const ModInt& a) { 
        if ((v += a.v) >= MOD) v -= MOD; 
        return *this; 
    }
    ModInt& operator-=(const ModInt& a) { 
        if ((v -= a.v) < 0) v += MOD; 
        return *this; 
    }
    ModInt& operator*=(const ModInt& a) { 
        v = v * a.v % MOD; 
        return *this; 
    }
    ModInt& operator/=(const ModInt& a) { 
        return *this *= a.inv(); 
    }

    // 四則演算子
    ModInt operator+(const ModInt& a) const { return ModInt(*this) += a; }
    ModInt operator-(const ModInt& a) const { return ModInt(*this) -= a; }
    ModInt operator*(const ModInt& a) const { return ModInt(*this) *= a; }
    ModInt operator/(const ModInt& a) const { return ModInt(*this) /= a; }

    // 符号反転・インクリメント・デクリメント
    ModInt operator-() const { return ModInt(v ? MOD - v : 0); }
    ModInt& operator++() { 
        if (++v == MOD) v = 0; 
        return *this; 
    }
    ModInt operator++(int) { 
        ModInt t = *this; 
        ++*this; 
        return t; 
    }
    ModInt& operator--() { 
        if (--v == -1) v = MOD - 1; 
        return *this; 
    }
    ModInt operator--(int) { 
        ModInt t = *this; 
        --*this; 
        return t; 
    }

    // 比較演算子
    bool operator==(const ModInt& a) const { return v == a.v; }
    bool operator!=(const ModInt& a) const { return v != a.v; }

    // 累乗 (x^n)
    ModInt pow(ll n) const {
        ModInt res = 1, x = *this;
        while (n > 0) {
            if (n & 1) res *= x;
            x *= x;
            n >>= 1;
        }
        return res;
    }

    // 逆元 (拡張ユークリッドの互除法)
    ModInt inv() const {
        ll a = v, b = MOD, u = 1, y = 0;
        while (b) {
            ll t = a / b;
            a -= t * b; swap(a, b);
            u -= t * y; swap(u, y);
        }
        return ModInt(u);
    }

    // 入出力
    friend istream& operator>>(istream& is, ModInt& a) { 
        ll t; is >> t; 
        a = ModInt(t); 
        return is; 
    }
    friend ostream& operator<<(ostream& os, const ModInt& a) { 
        return os << a.v; 
    }

    // --- ここから二項係数（テーブル管理） ---
    static inline vector<ModInt> fact_table, inv_fact_table;

    // テーブルの動的構築
    static void build(ll n) {
        ll sz = fact_table.size();
        if (sz > n) return;
        if (sz == 0) {
            fact_table.assign(1, 1);
            inv_fact_table.assign(1, 1);
            sz = 1;
        }
        fact_table.resize(n + 1);
        inv_fact_table.resize(n + 1);
        for (ll i = sz; i <= n; i++) fact_table[i] = fact_table[i - 1] * i;
        inv_fact_table[n] = fact_table[n].inv();
        for (ll i = n - 1; i >= sz; i--) inv_fact_table[i] = inv_fact_table[i + 1] * (i + 1);
    }

    // 階乗 n!
    static ModInt fact(ll n) {
        if (n < 0) return 0;
        build(n);
        return fact_table[n];
    }

    // 階乗の逆元 (n!)^-1
    static ModInt inv_fact(ll n) {
        if (n < 0) return 0;
        build(n);
        return inv_fact_table[n];
    }

    // 組合せ nCr
    static ModInt nCr(ll n, ll r) {
        if (n < r || n < 0 || r < 0) return 0;
        build(n);
        return fact_table[n] * inv_fact_table[r] * inv_fact_table[n - r];
    }

    // 順列 nPr
    static ModInt nPr(ll n, ll r) {
        if (n < r || n < 0 || r < 0) return 0;
        build(n);
        return fact_table[n] * inv_fact_table[n - r];
    }

    // 重複組合せ nHr
    static ModInt nHr(ll n, ll r) {
        if (n == 0 && r == 0) return 1;
        if (n <= 0 || r < 0) return 0;
        return nCr(n + r - 1, r);
    }
};