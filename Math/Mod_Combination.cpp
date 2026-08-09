// 任意のMod整数型(mint等)をテンプレートとして受け取り、組合せを計算するライブラリ
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

template <class T>
struct Mod_Combination {
    vector<T> _fact, _inv_fact;

    // コンストラクタ (最大値 max_n までテーブルを事前構築)
    Mod_Combination(ll max_n) {
        _fact.resize(max_n + 1);
        _inv_fact.resize(max_n + 1);
        
        _fact[0] = 1;
        for (ll i = 1; i <= max_n; i++) {
            _fact[i] = _fact[i - 1] * i;
        }
        
        _inv_fact[max_n] = _fact[max_n].inv();
        for (ll i = max_n - 1; i >= 0; i--) {
            _inv_fact[i] = _inv_fact[i + 1] * (i + 1);
        }
    }

    // 階乗 n!
    T fact(ll n) const {
        if (n < 0 || n >= (ll)_fact.size()) return 0;
        return _fact[n];
    }

    // 階乗の逆元 (n!)^-1
    T inv_fact(ll n) const {
        if (n < 0 || n >= (ll)_inv_fact.size()) return 0;
        return _inv_fact[n];
    }

    // 組合せ nCr (前処理 O(N), クエリ O(1))
    T nCr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        return _fact[n] * _inv_fact[r] * _inv_fact[n - r];
    }

    // 巨大な n と小さな r (r <= max_n) に対する組合せ nCr (クエリ O(r))
    T nCr_large_n(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        if (r > n - r) r = n - r;
        if (r >= (ll)_inv_fact.size()) return 0;
        
        T res = 1;
        for (ll i = 0; i < r; i++) {
            res *= T(n - i);
        }
        return res * _inv_fact[r];
    }

    // 順列 nPr
    T nPr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        return _fact[n] * _inv_fact[n - r];
    }

    // 重複組合せ nHr
    T nHr(ll n, ll r) const {
        if (n < 0 || r < 0) return 0;
        if (n == 0 && r == 0) return 1;
        return nCr(n + r - 1, r);
    }

    // 多項係数
    T multinomial(const vector<ll>& k) const {
        ll sum = 0;
        for (ll x : k) {
            if (x < 0) return 0;
            sum += x;
        }
        
        T res = fact(sum);
        for (ll x : k) {
            res *= inv_fact(x);
        }
        return res;
    }
};