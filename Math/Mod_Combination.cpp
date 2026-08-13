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

template <typename T = ll>
struct ModCombination {
    vector<T> _fact;
    vector<T> _inv_fact;

    // max_nまでの階乗とその逆元を事前計算するコンストラクタ
    ModCombination(ll max_n) {
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

    // 階乗 n! を返す
    T fact(ll n) const {
        if (n < 0 || n >= (ll)_fact.size()) {
            return 0;
        }
        return _fact[n];
    }

    // 階乗の逆元 (n!)^-1 を返す
    T inv_fact(ll n) const {
        if (n < 0 || n >= (ll)_inv_fact.size()) {
            return 0;
        }
        return _inv_fact[n];
    }

    // 組合せ nCr を返す (計算量: O(1))
    T nCr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) {
            return 0;
        }
        return _fact[n] * _inv_fact[r] * _inv_fact[n - r];
    }

    // nが巨大でrが小さい場合の組合せ nCr を返す (計算量: O(r))
    T nCr_large_n(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) {
            return 0;
        }
        if (r > n - r) {
            r = n - r;
        }
        if (r >= (ll)_inv_fact.size()) {
            return 0;
        }
        
        T res = 1;
        for (ll i = 0; i < r; i++) {
            res *= T(n - i);
        }
        return res * _inv_fact[r];
    }

    // 順列 nPr を返す (計算量: O(1))
    T nPr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) {
            return 0;
        }
        return _fact[n] * _inv_fact[n - r];
    }

    // 重複組合せ nHr を返す (計算量: O(1))
    T nHr(ll n, ll r) const {
        if (n < 0 || r < 0) {
            return 0;
        }
        if (n == 0 && r == 0) {
            return 1;
        }
        return nCr(n + r - 1, r);
    }

    // 多項係数を返す
    T multinomial(const vector<ll>& k) const {
        ll sum = 0;
        for (ll x : k) {
            if (x < 0) {
                return 0;
            }
            sum += x;
        }
        
        T res = fact(sum);
        for (ll x : k) {
            res *= inv_fact(x);
        }
        return res;
    }
};