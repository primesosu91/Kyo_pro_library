// 要素のグループ分けと結合を高速に行うUnion-Find
//
// example:
//
// yosupo judge - Unionfind
// https://judge.yosupo.jp/submission/391279
//
// ABC 420 E - Reachability Query (1Q)
// https://atcoder.jp/contests/abc420/submissions/78173963
// 連結成分に黒頂点が含まれるか判定
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct UnionFind {
    // 根の場合は「-グループの要素数」、子の場合は「親のインデックス」を保持する
    vector<ll> par;
    
    // 現在のグループ数（連結成分の個数）
    ll group_count;

    UnionFind(ll n) {
        par.assign(n, -1);
        group_count = n;
    }

    ll root(ll x) {
        if (par[x] < 0) {
            return x;
        }
        
        // 経路圧縮：根を直接親に繋ぎ直す
        return par[x] = root(par[x]);
    }

    bool same(ll x, ll y) {
        return root(x) == root(y);
    }

    bool unite(ll x, ll y) {
        ll rx = root(x);
        ll ry = root(y);
        
        if (rx == ry) {
            return false;
        }
        
        // サイズによる結合（要素数が大きい方に小さい方を結合する）
        if (par[rx] > par[ry]) {
            swap(rx, ry);
        }
        
        par[rx] += par[ry];
        par[ry] = rx;
        group_count--;
        
        return true;
    }

    ll size(ll x) {
        return -par[root(x)];
    }

    ll count() {
        return group_count;
    }

    vector<vector<ll>> groups() {
        ll n = par.size();
        vector<vector<ll>> res(n);
        
        for (ll i = 0; i < n; i++) {
            res[root(i)].push_back(i);
        }
        
        // 空でないグループのみを抽出して返す
        vector<vector<ll>> valid_res;
        for (ll i = 0; i < n; i++) {
            if (!res[i].empty()) {
                valid_res.push_back(res[i]);
            }
        }
        
        return valid_res;
    }
};