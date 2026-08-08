// 燃やす埋める問題
//
// example:
//
// ARC 085 E - MUL// 
// https://atcoder.jp/contests/arc085/submissions/78185519
// 割るか割らないか問題
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Cap = ll>
struct ProjectSelection {
    struct _Edge {
        ll to;
        Cap cap;
        ll rev;
    };

    ll n, S, T;
    Cap base_profit;
    vector<vector<_Edge>> g;
    vector<ll> level, iter;

    // 初期化 (n: 要素数)
    ProjectSelection(ll n) : n(n), S(n), T(n + 1), base_profit(0), g(n + 2), level(n + 2), iter(n + 2) {}

    // 内部用の辺追加関数
    void _add_edge(ll from, ll to, Cap cap) {
        if (cap == 0) return;
        ll from_id = g[from].size();
        ll to_id = g[to].size();
        if (from == to) to_id++;
        g[from].push_back({to, cap, to_id});
        g[to].push_back({from, 0, from_id});
    }

    void _add_cost_internal(ll v, Cap cost0, Cap cost1) {
        _add_edge(S, v, cost1);
        _add_edge(v, T, cost0);
    }

    // 要素 v が 0 または 1 を選んだときの「利益」を追加（正負どちらも可）
    void add_profit(ll v, Cap profit0, Cap profit1) {
        Cap p_max = max(profit0, profit1);
        base_profit += p_max;
        _add_cost_internal(v, p_max - profit0, p_max - profit1);
    }

    // 要素 v が 0 または 1 を選んだときの「コスト」を追加（正負どちらも可）
    void add_cost(ll v, Cap cost0, Cap cost1) {
        Cap c_min = min(cost0, cost1);
        base_profit -= c_min;
        _add_cost_internal(v, cost0 - c_min, cost1 - c_min);
    }

    // 要素 u が 0 かつ 要素 v が 1 の場合に発生するコスト（ペナルティ）を追加
    // ※ 2変数間の負のペナルティは劣モジュラ性を満たさず最大流で解けないためアサーションで弾く
    void add_penalty(ll u, ll v, Cap cost) {
        assert(cost >= 0);
        _add_edge(u, v, cost);
    }

    // 残余グラフ上で始点からの距離を計算
    void bfs(ll s) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<ll> q;
        q.push(s);
        while (!q.empty()) {
            ll v = q.front();
            q.pop();
            for (auto& e : g[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }

    // 増加パスを探索してフローを流す
    Cap dfs(ll v, ll t, Cap up) {
        if (v == t) return up;
        Cap res = 0;
        ll level_v = level[v];
        for (ll& i = iter[v]; i < (ll)g[v].size(); i++) {
            _Edge& e = g[v][i];
            if (level_v < level[e.to] && e.cap > 0) {
                Cap d = dfs(e.to, t, min(up - res, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    res += d;
                    if (res == up) return res;
                }
            }
        }
        return res;
    }

    // 最大利益を計算
    Cap build() {
        Cap flow = 0;
        while (true) {
            bfs(S);
            if (level[T] < 0) return base_profit - flow;
            fill(iter.begin(), iter.end(), 0);
            Cap f = dfs(S, T, numeric_limits<Cap>::max());
            while (f > 0) {
                flow += f;
                f = dfs(S, T, numeric_limits<Cap>::max());
            }
        }
    }

    // 要素 v が最終的にどちらを選択したか（0 または 1）を取得
    ll get(ll v) {
        return level[v] < 0 ? 1 : 0;
    }

    // 全要素の選択状態を取得
    vector<ll> get_all() {
        vector<ll> res(n);
        for (ll i = 0; i < n; i++) {
            res[i] = get(i);
        }
        return res;
    }
};