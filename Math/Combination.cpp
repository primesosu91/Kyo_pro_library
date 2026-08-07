// 階乗・二項係数などの前処理・O(1)クエリ
//
// example:
//
// yosupo judge - Binomial Coefficient (Prime Mod)
// https://judge.yosupo.jp/submission/391273
// 素数modの二項係数前計算
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Combination {
private:
    ll mod;
    vector<ll> _fact, _inv_fact, _inv;

    ll power(ll base, ll exp) const {
        ll res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return res;
    }

public:
    Combination(ll max_n, ll mod_ = 998244353) : mod(mod_) {
        _fact.assign(max_n + 1, 0);
        _inv_fact.assign(max_n + 1, 0);
        _inv.assign(max_n + 1, 0);

        _fact[0] = _fact[1] = 1;
        _inv_fact[0] = _inv_fact[1] = 1;
        _inv[1] = 1;

        for (ll i = 2; i <= max_n; i++) {
            _fact[i] = _fact[i - 1] * i % mod;
            _inv[i] = mod - _inv[mod % i] * (mod / i) % mod;
            _inv_fact[i] = _inv_fact[i - 1] * _inv[i] % mod;
        }
    }

    ll fact(ll n) const {
        if (n < 0 || n >= (ll)_fact.size()) return 0;
        return _fact[n];
    }

    ll inv_fact(ll n) const {
        if (n < 0 || n >= (ll)_inv_fact.size()) return 0;
        return _inv_fact[n];
    }

    ll inv(ll n) const {
        if (n < 0 || n >= (ll)_inv.size()) return 0;
        return _inv[n];
    }

    ll nCr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        return _fact[n] * _inv_fact[r] % mod * _inv_fact[n - r] % mod;
    }

    ll nPr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        return _fact[n] * _inv_fact[n - r] % mod;
    }

    ll nHr(ll n, ll r) const {
        if (n < 0 || r < 0) return 0;
        if (n == 0 && r == 0) return 1;
        return nCr(n + r - 1, r);
    }

    ll nCr_large_n(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        if (r == 0) return 1;
        if (r > n - r) r = n - r;
        
        ll res = 1;
        for (ll i = 0; i < r; i++) {
            res = res * ((n - i) % mod) % mod;
        }
        return res * _inv_fact[r] % mod;
    }

    ll multinomial(const vector<ll>& k) const {
        ll sum = 0;
        for (ll x : k) {
            if (x < 0) return 0;
            sum += x;
        }
        
        ll res = fact(sum);
        for (ll x : k) {
            res = res * inv_fact(x) % mod;
        }
        return res;
    }
};