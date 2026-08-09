// 実行時にMod値を決定・変更できるMod整数ライブラリ
//
// example:
//
// yosupo judge - Binomial Coefficient (Prime Mod)
// https://judge.yosupo.jp/submission/391656
// 任意mod二項係数
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DynamicModInt {
    ll v;
    static inline ll MOD = 998244353; // デフォルト値

    // Mod値の設定・取得
    static void set_mod(ll m) { MOD = m; }
    static ll mod() { return MOD; }

    // コンストラクタ (負の数も自動で正規化)
    DynamicModInt(ll x = 0) : v(x % MOD) {
        if (v < 0) v += MOD;
    }

    // 値を取得
    ll val() const { return v; }

    // 複合代入演算子
    DynamicModInt& operator+=(const DynamicModInt& a) {
        if ((v += a.v) >= MOD) v -= MOD;
        return *this;
    }
    DynamicModInt& operator-=(const DynamicModInt& a) {
        if ((v -= a.v) < 0) v += MOD;
        return *this;
    }
    DynamicModInt& operator*=(const DynamicModInt& a) {
        v = v * a.v % MOD;
        return *this;
    }
    DynamicModInt& operator/=(const DynamicModInt& a) {
        return *this *= a.inv();
    }

    // 四則演算子
    DynamicModInt operator+(const DynamicModInt& a) const { return DynamicModInt(*this) += a; }
    DynamicModInt operator-(const DynamicModInt& a) const { return DynamicModInt(*this) -= a; }
    DynamicModInt operator*(const DynamicModInt& a) const { return DynamicModInt(*this) *= a; }
    DynamicModInt operator/(const DynamicModInt& a) const { return DynamicModInt(*this) /= a; }

    // 単項演算子・インクリメント・デクリメント
    DynamicModInt operator-() const { return DynamicModInt(v ? MOD - v : 0); }
    DynamicModInt& operator++() {
        if (++v == MOD) v = 0;
        return *this;
    }
    DynamicModInt operator++(int) {
        DynamicModInt t = *this;
        ++*this;
        return t;
    }
    DynamicModInt& operator--() {
        if (--v == -1) v = MOD - 1;
        return *this;
    }
    DynamicModInt operator--(int) {
        DynamicModInt t = *this;
        --*this;
        return t;
    }

    // 比較演算子
    bool operator==(const DynamicModInt& a) const { return v == a.v; }
    bool operator!=(const DynamicModInt& a) const { return v != a.v; }

    // 累乗 (x^n)
    DynamicModInt pow(ll n) const {
        DynamicModInt res = 1, x = *this;
        while (n > 0) {
            if (n & 1) res *= x;
            x *= x;
            n >>= 1;
        }
        return res;
    }

    // 逆元 (拡張ユークリッドの互除法)
    DynamicModInt inv() const {
        ll a = v, b = MOD, u = 1, y = 0;
        while (b) {
            ll t = a / b;
            a -= t * b; swap(a, b);
            u -= t * y; swap(u, y);
        }
        return DynamicModInt(u);
    }

    // 入出力
    friend istream& operator>>(istream& is, DynamicModInt& a) {
        ll t; is >> t;
        a = DynamicModInt(t);
        return is;
    }
    friend ostream& operator<<(ostream& os, const DynamicModInt& a) {
        return os << a.v;
    }
};