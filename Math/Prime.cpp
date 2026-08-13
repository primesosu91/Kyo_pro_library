// 範囲前処理と単一判定ライブラリ
//
// example:
//
// yosupo judge - Primality Test
// https://judge.yosupo.jp/submission/392083
// 素数判定
//
// yosupo judge - Counting Primes
// https://judge.yosupo.jp/submission/392084
// n以下の素数の個数
//
// yosupo judge - Factorize
// https://judge.yosupo.jp/submission/392086
// 素因数分解
//
// yosupo judge - Primitive Root
// https://judge.yosupo.jp/submission/392091
// 原始根
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 最大公約数を求めるユーティリティ関数
ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// 繰り返し二乗法（__int128_tを用いてオーバーフローを防止）
ll mod_pow(ll a, ll n, ll mod) {
    ll res = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) res = (ll)((__int128_t)res * a % mod);
        a = (ll)((__int128_t)a * a % mod);
        n >>= 1;
    }
    return res;
}

// 事前構築テーブルを用いた素数・数論クエリ処理（小〜中規模のクエリ向け）
struct PrimeTable {
    vector<ll> min_factor;
    vector<ll> primes;

    // Nまでの篩を線形篩(O(N))により構築
    PrimeTable(ll n) {
        min_factor.assign(n + 1, 0);
        for (ll i = 2; i <= n; i++) {
            if (min_factor[i] == 0) {
                min_factor[i] = i;
                primes.push_back(i);
            }
            for (ll p : primes) {
                if (p > min_factor[i] || i * p > n) break;
                min_factor[i * p] = p;
            }
        }
    }

    // xが素数か判定 (O(1))
    bool is_prime(ll x) const {
        if (x < 2) return false;
        return min_factor[x] == x;
    }

    // 構築した範囲内の素数リストを取得
    vector<ll> get_primes() const {
        return primes;
    }

    // x以下の素数の個数を取得 (O(log(素数の数)))
    ll get_prime_count(ll x) const {
        return upper_bound(primes.begin(), primes.end(), x) - primes.begin();
    }

    // 高速素因数分解 (O(log x))
    vector<pair<ll, ll>> get_factors(ll x) const {
        vector<pair<ll, ll>> res;
        while (x > 1) {
            ll p = min_factor[x];
            ll count = 0;
            while (min_factor[x] == p) {
                count++;
                x /= p;
            }
            res.push_back({p, count});
        }
        return res;
    }

    // 約数リストを昇順で取得
    vector<ll> get_divisors(ll x) const {
        auto factors = get_factors(x);
        vector<ll> res = {1};
        for (auto p : factors) {
            ll prime = p.first;
            ll count = p.second;
            ll sz = res.size();
            ll mul = 1;
            for (ll i = 0; i < count; i++) {
                mul *= prime;
                for (ll j = 0; j < sz; j++) {
                    res.push_back(res[j] * mul);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    // オイラーのφ関数 (O(log x))
    ll get_euler(ll x) const {
        auto factors = get_factors(x);
        ll res = x;
        for (auto p : factors) {
            res = res / p.first * (p.first - 1);
        }
        return res;
    }

    // gが法pの原始根か判定（pは素数）
    bool is_primitive_root(ll g, ll p) const {
        if (p == 2) return g == 1;
        if (g % p == 0) return false;
        auto factors = get_factors(p - 1);
        for (auto f : factors) {
            if (mod_pow(g, (p - 1) / f.first, p) == 1) return false;
        }
        return true;
    }

    // 法pの最小の原始根を取得（pは素数）
    ll get_primitive_root(ll p) const {
        if (p == 2) return 1;
        for (ll g = 2; g < p; g++) {
            if (is_primitive_root(g, p)) return g;
        }
        return -1;
    }
};

// 単一の数に対する素数判定（Miller-Rabin法: O(log n)）
bool is_prime(ll n) {
    if (n <= 1) return false;
    if (n == 2 || n == 3 || n == 5) return true;
    if (n % 2 == 0) return false;
    ll d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }
    vector<ll> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (ll a : bases) {
        if (n <= a) break;
        ll t = d;
        ll y = mod_pow(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = (ll)((__int128_t)y * y % n);
            t <<= 1;
        }
        if (y != n - 1 && t % 2 == 0) return false;
    }
    return true;
}

// 単一の数に対する素因数分解の内部処理（Pollard's rho法）
ll pollard_rho(ll n) {
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;
    for (ll c = 1; ; c++) {
        auto f = [&](ll x) { return (ll)(((__int128_t)x * x + c) % n); };
        ll x = 2, y = 2, g = 1;
        while (g == 1) {
            x = f(x);
            y = f(f(y));
            g = gcd(abs(x - y), n);
        }
        if (g < n) return g;
    }
}

// 素因数分解の再帰呼び出し部分
void factorize_impl(ll n, vector<ll>& res) {
    if (n <= 1) return;
    if (is_prime(n)) {
        res.push_back(n);
        return;
    }
    ll p = pollard_rho(n);
    factorize_impl(p, res);
    factorize_impl(n / p, res);
}

// 単一の数に対する高速素因数分解 (O(n^(1/4)))
vector<pair<ll, ll>> get_factors(ll n) {
    vector<ll> primes;
    factorize_impl(n, primes);
    sort(primes.begin(), primes.end());
    vector<pair<ll, ll>> res;
    for (ll p : primes) {
        if (!res.empty() && res.back().first == p) {
            res.back().second++;
        } else {
            res.push_back({p, 1});
        }
    }
    return res;
}

// 単一の数に対する約数リスト取得（昇順）
vector<ll> get_divisors(ll n) {
    auto factors = get_factors(n);
    vector<ll> res = {1};
    for (auto p : factors) {
        ll prime = p.first;
        ll count = p.second;
        ll sz = res.size();
        ll mul = 1;
        for (ll i = 0; i < count; i++) {
            mul *= prime;
            for (ll j = 0; j < sz; j++) {
                res.push_back(res[j] * mul);
            }
        }
    }
    sort(res.begin(), res.end());
    return res;
}

// 単一の数に対するオイラーのφ関数 (O(n^(1/4)))
ll get_euler(ll n) {
    auto factors = get_factors(n);
    ll res = n;
    for (auto p : factors) {
        res = res / p.first * (p.first - 1);
    }
    return res;
}

// 単一の巨大な素数に対する原始根判定
bool is_primitive_root(ll g, ll p) {
    if (p == 2) return g == 1;
    if (g % p == 0) return false;
    auto factors = get_factors(p - 1);
    for (auto f : factors) {
        if (mod_pow(g, (p - 1) / f.first, p) == 1) return false;
    }
    return true;
}

// 単一の巨大な素数に対する最小の原始根取得
ll get_primitive_root(ll p) {
    if (p == 2) return 1;
    for (ll g = 2; g < p; g++) {
        if (is_primitive_root(g, p)) return g;
    }
    return -1;
}

// N以下の素数の個数を取得（Lucy Hedgehog アルゴリズム: O(n^(3/4))）
ll get_prime_count(ll n) {
    if (n <= 1) return 0;
    ll sq = sqrt(n);
    vector<ll> sm(sq + 1), la(sq + 1);
    for (ll i = 1; i <= sq; i++) {
        sm[i] = i - 1;
        la[i] = n / i - 1;
    }
    for (ll p = 2; p <= sq; p++) {
        if (sm[p] == sm[p - 1]) continue;
        ll p_cnt = sm[p - 1];
        ll p2 = p * p;
        ll end = min(sq, n / p2);
        for (ll i = 1; i <= end; i++) {
            ll d = i * p;
            if (d <= sq) la[i] -= la[d] - p_cnt;
            else la[i] -= sm[n / d] - p_cnt;
        }
        for (ll i = sq; i >= p2; i--) {
            sm[i] -= sm[i / p] - p_cnt;
        }
    }
    return la[1];
}