// モノイドがのるセグメント木
//
// example:
//
// yosupo judge - Point Add Range Sum
// https://judge.yosupo.jp/submission/391291
// 一点更新区間和
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T, T (*op)(T, T), T (*e)()>
struct SegmentTree {
    ll n, size;
    vector<T> d;

    // 内部用のノード更新関数
    void update_node(ll k) {
        d[k] = op(d[2 * k], d[2 * k + 1]);
    }

    // サイズ n で初期化
    SegmentTree(ll n) : SegmentTree(vector<T>(n, e())) {}

    // 配列 v で初期化 (O(N)で構築)
    SegmentTree(const vector<T>& v) {
        n = v.size();
        size = 1;
        while (size < n) size *= 2;
        d.assign(2 * size, e());
        for (ll i = 0; i < n; i++) d[size + i] = v[i];
        for (ll i = size - 1; i >= 1; i--) update_node(i);
    }

    // 位置 p の値を x に更新 (0-indexed)
    void set(ll p, T x) {
        p += size;
        d[p] = x;
        while (p > 1) {
            p /= 2;
            update_node(p);
        }
    }

    // 位置 p に x を作用 (d[p] = op(d[p], x))
    void add(ll p, T x) {
        set(p, op(get(p), x));
    }

    // 位置 p の要素を取得
    T get(ll p) {
        return d[p + size];
    }

    // 半開区間 [l, r) の演算結果を取得
    T get(ll l, ll r) {
        T sml = e(), smr = e();
        l += size;
        r += size;
        while (l < r) {
            if (l % 2 == 1) sml = op(sml, d[l++]);
            if (r % 2 == 1) smr = op(d[--r], smr);
            l /= 2;
            r /= 2;
        }
        return op(sml, smr);
    }

    // 全区間 [0, n) の演算結果を取得
    T get_all() {
        return d[1];
    }

    // 条件 f を満たす最大の r を二分探索で取得
    template <class F>
    ll max_right(ll l, F f) {
        if (l == n) return n;
        l += size;
        T sm = e();
        do {
            while (l % 2 == 0) l /= 2;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    // 条件 f を満たす最小の l を二分探索で取得
    template <class F>
    ll min_left(ll r, F f) {
        if (r == 0) return 0;
        r += size;
        T sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r /= 2;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};