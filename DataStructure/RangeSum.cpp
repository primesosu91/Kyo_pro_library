// 2次元累積和・2次元いもす法ライブラリ（1次元対応）
//
// example:
//
// Static Range Sum
// https://judge.yosupo.jp/submission/391107
// 静的区間和
//
// ABC 331 D - Tile Pattern (1Q)
// https://atcoder.jp/contests/abc331/submissions/78158981
// 2次元累積和
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template<typename T = ll>
struct RangeSum {
    ll h, w;
    vector<vector<T>> data;
    bool built;

    // サイズ指定コンストラクタ（全要素0）
    RangeSum(ll h, ll w) : h(h), w(w), built(false) {
        data.assign(h + 2, vector<T>(w + 2, 0));
    }

    // 2次元配列からの構築コンストラクタ
    RangeSum(const vector<vector<T>>& v) : built(false) {
        h = v.size();
        w = (h > 0 ? v[0].size() : 0);
        data.assign(h + 2, vector<T>(w + 2, 0));
        for (ll i = 0; i < h; ++i) {
            for (ll j = 0; j < w; ++j) {
                data[i + 1][j + 1] = v[i][j];
            }
        }
        build();
    }

    // 1次元配列からの構築コンストラクタ
    RangeSum(const vector<T>& v) : built(false) {
        h = v.size();
        w = 1;
        data.assign(h + 2, vector<T>(w + 2, 0));
        for (ll i = 0; i < h; ++i) {
            data[i + 1][1] = v[i];
        }
        build();
    }

    // 2次元：点 (x, y) に val を加算
    void add(ll x, ll y, T val) {
        data[x + 1][y + 1] += val;
        built = false;
    }

    // 2次元：区間 [x1, x2) × [y1, y2) に val を加算（いもす法）
    void add(ll x1, ll y1, ll x2, ll y2, T val) {
        data[x1 + 1][y1 + 1] += val;
        data[x2 + 1][y1 + 1] -= val;
        data[x1 + 1][y2 + 1] -= val;
        data[x2 + 1][y2 + 1] += val;
        built = false;
    }

    // 2次元：点 (x, y) の値を val に変更
    void set(ll x, ll y, T val) {
        data[x + 1][y + 1] = val;
        built = false;
    }

    // 1次元：点 x に val を加算
    void add_1d(ll x, T val) {
        add(x, 0, val);
    }

    // 1次元：区間 [x1, x2) に val を加算
    void add_1d(ll x1, ll x2, T val) {
        add(x1, 0, x2, 1, val);
    }

    // 1次元：点 x の値を val に変更
    void set_1d(ll x, T val) {
        set(x, 0, val);
    }

    // 累積和配列の構築
    void build() {
        if (built) return;
        for (ll i = 1; i <= h + 1; ++i) {
            for (ll j = 1; j <= w + 1; ++j) {
                data[i][j] += data[i - 1][j] + data[i][j - 1] - data[i - 1][j - 1];
            }
        }
        built = true;
    }

    // 2次元：点 (x, y) の値を取得
    T get(ll x, ll y) {
        if (!built) build();
        return data[x + 1][y + 1];
    }

    // 2次元：区間 [x1, x2) × [y1, y2) の和を取得
    T get(ll x1, ll y1, ll x2, ll y2) {
        if (!built) build();
        return data[x2][y2] - data[x1][y2] - data[x2][y1] + data[x1][y1];
    }

    // 1次元：点 x の値を取得
    T get_1d(ll x) {
        return get(x, 0);
    }

    // 1次元：区間 [x1, x2) の和を取得
    T get_1d(ll x1, ll x2) {
        return get(x1, 0, x2, 1);
    }

    // 構築後の全データグリッドを取得
    vector<vector<T>> get_grid() {
        if (!built) build();
        vector<vector<T>> res(h, vector<T>(w));
        for (ll i = 0; i < h; ++i) {
            for (ll j = 0; j < w; ++j) {
                res[i][j] = data[i + 1][j + 1];
            }
        }
        return res;
    }
};