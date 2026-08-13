// 四則演算や逆元に対応したMod整数ライブラリ
//
// example:
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <ll MOD = 998244353>
struct ModInt {
    ll v;

    // コンストラクタ (負の数も自動で正規化)
    ModInt(ll x = 0) : v(x % MOD) {
        if (v < 0) {
            v += MOD;
        }
    }

    // 値の取得
    ll val() const {
        return v;
    }

    // MODの取得
    ll mod() const {
        return MOD;
    }

    // 加算と代入
    ModInt& operator+=(const ModInt& a) {
        if ((v += a.v) >= MOD) {
            v -= MOD;
        }
        return *this;
    }

    // 減算と代入
    ModInt& operator-=(const ModInt& a) {
        if ((v -= a.v) < 0) {
            v += MOD;
        }
        return *this;
    }

    // 乗算と代入
    ModInt& operator*=(const ModInt& a) {
        v = v * a.v % MOD;
        return *this;
    }

    // 除算と代入
    ModInt& operator/=(const ModInt& a) {
        return *this *= a.inv();
    }

    // 四則演算子
    ModInt operator+(const ModInt& a) const {
        return ModInt(*this) += a;
    }

    ModInt operator-(const ModInt& a) const {
        return ModInt(*this) -= a;
    }

    ModInt operator*(const ModInt& a) const {
        return ModInt(*this) *= a;
    }

    ModInt operator/(const ModInt& a) const {
        return ModInt(*this) /= a;
    }

    // 単項演算子 (符号反転)
    ModInt operator-() const {
        return ModInt(v ? MOD - v : 0);
    }

    // 前置インクリメント
    ModInt& operator++() {
        if (++v == MOD) {
            v = 0;
        }
        return *this;
    }

    // 後置インクリメント
    ModInt operator++(int) {
        ModInt t = *this;
        ++*this;
        return t;
    }

    // 前置デクリメント
    ModInt& operator--() {
        if (--v == -1) {
            v = MOD - 1;
        }
        return *this;
    }

    // 後置デクリメント
    ModInt operator--(int) {
        ModInt t = *this;
        --*this;
        return t;
    }

    // 比較演算子 (等価)
    bool operator==(const ModInt& a) const {
        return v == a.v;
    }

    // 比較演算子 (非等価)
    bool operator!=(const ModInt& a) const {
        return v != a.v;
    }

    // 累乗 (繰り返し二乗法)
    ModInt pow(ll n) const {
        ModInt res = 1, x = *this;
        while (n > 0) {
            if (n & 1) {
                res *= x;
            }
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
            a -= t * b;
            swap(a, b);
            u -= t * y;
            swap(u, y);
        }
        return ModInt(u);
    }

    // 入力ストリーム
    friend istream& operator>>(istream& is, ModInt& a) {
        ll t;
        is >> t;
        a = ModInt(t);
        return is;
    }

    // 出力ストリーム
    friend ostream& operator<<(ostream& os, const ModInt& a) {
        return os << a.v;
    }
};