// 任意Mod用 二項係数・順列等の計算
//
// example:
//
// yosupo judge - Binomial Coefficient
// https://judge.yosupo.jp/submission/391278
// 任意のmodの二項係数
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct CombinationMod {
private:
    struct PrimeInfo {
        ll p;
        ll pe;
        ll e;
        ll crt_coef;
    };
    
    ll mod;
    vector<PrimeInfo> primes; 
    vector<vector<ll>> fact_pe;  

    ll extgcd(ll a, ll b, ll &x, ll &y) const {
        if (b == 0) { x = 1; y = 0; return a; }
        ll x1, y1;
        ll d = extgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return d;
    }

    ll inv(ll a, ll m) const {
        ll x, y;
        extgcd(a, m, x, y);
        return (x % m + m) % m;
    }

    ll get_fact_pe_cnt(ll n, int idx, ll& p_cnt) const {
        ll p = primes[idx].p;
        ll pe = primes[idx].pe;
        ll res = 1;
        ll sign = 1;
        ll block = fact_pe[idx][pe - 1]; // ウィルソンの定理拡張により必ず 1 or pe-1
        ll cnt = 0;
        
        while (n > 0) {
            ll cycles = n / pe;
            ll rem = n % pe;
            
            // block が -1 (pe-1) で、サイクル数が奇数の時のみ符号反転
            if (block == pe - 1 && (cycles % 2 != 0)) {
                sign = -sign;
            }
            
            res = res * fact_pe[idx][rem] % pe;
            n /= p;
            cnt += n;
        }
        
        if (sign == -1) res = (pe - res) % pe;
        p_cnt = cnt;
        return res;
    }

    ll evaluate(ll n_val, const vector<ll>& den_vals, int idx) const {
        ll p = primes[idx].p;
        ll pe = primes[idx].pe;
        ll e = primes[idx].e;
        
        ll cnt_n = 0;
        ll num = get_fact_pe_cnt(n_val, idx, cnt_n);
        
        ll cnt_den = 0;
        ll den = 1;
        for (ll x : den_vals) {
            ll c = 0;
            den = den * get_fact_pe_cnt(x, idx, c) % pe;
            cnt_den += c;
        }
        
        ll cnt = cnt_n - cnt_den;
        // p で e 回以上割り切れるなら、pe で割った余りは確実に 0
        if (cnt >= e) return 0;
        
        ll p_pow = 1;
        for (ll i = 0; i < cnt; i++) p_pow *= p;
        
        ll res = num * inv(den, pe) % pe;
        res = res * p_pow % pe;
        return res;
    }

    template<typename Func>
    ll crt_solve(Func func) const {
        if (mod == 1) return 0;
        ll ans = 0;
        for (int i = 0; i < (int)primes.size(); i++) {
            ll rem = func(i);
            // 前計算済みの CRT 係数を利用し O(1) 処理
            ans = (ans + rem * primes[i].crt_coef) % mod;
        }
        return ans;
    }

public:
    CombinationMod(ll m) : mod(m) {
        ll temp = m;
        for (ll i = 2; i * i <= temp; i++) {
            if (temp % i == 0) {
                ll pe = 1, e = 0;
                while (temp % i == 0) {
                    pe *= i;
                    temp /= i;
                    e++;
                }
                primes.push_back({i, pe, e, 0});
            }
        }
        if (temp > 1) {
            primes.push_back({temp, temp, 1, 0});
        }
        
        // CRT係数の完全前処理
        for (auto& info : primes) {
            ll m_other = mod / info.pe;
            ll inv_m = inv(m_other, info.pe);
            info.crt_coef = (inv_m * m_other) % mod;
        }
        
        int k = primes.size();
        fact_pe.resize(k);
        for (int i = 0; i < k; i++) {
            ll p = primes[i].p;
            ll pe = primes[i].pe;
            fact_pe[i].assign(pe, 1);
            for (ll j = 1; j < pe; j++) {
                if (j % p == 0) fact_pe[i][j] = fact_pe[i][j - 1];
                else fact_pe[i][j] = fact_pe[i][j - 1] * j % pe;
            }
        }
    }

    ll nCr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        if (r == 0 || r == n) return 1;
        return crt_solve([&](int i) { return evaluate(n, {r, n - r}, i); });
    }

    ll nPr(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        if (r == 0) return 1;
        return crt_solve([&](int i) { return evaluate(n, {n - r}, i); });
    }

    ll nHr(ll n, ll r) const {
        if (n < 0 || r < 0) return 0;
        if (n == 0 && r == 0) return 1;
        return nCr(n + r - 1, r);
    }

    ll multinomial(const vector<ll>& k) const {
        ll sum = 0;
        for (ll x : k) {
            if (x < 0) return 0;
            sum += x;
        }
        if (sum == 0) return 1;
        return crt_solve([&](int i) { return evaluate(sum, k, i); });
    }
};