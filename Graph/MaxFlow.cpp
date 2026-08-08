// 最小カット・最大流を求めるDinic法
//
// example:
//
// yosupo judge - Matching on Bipartite Graph
// https://judge.yosupo.jp/submission/391383
// 二部グラフ最大マッチング
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Cap = ll>
struct MaxFlow {
    struct _Edge {
        ll to;
        Cap cap;
        ll rev;
    };

    struct Edge {
        ll from;
        ll to;
        Cap cap;
        Cap flow;
    };

    ll n;
    vector<pair<ll, ll>> pos;
    vector<vector<_Edge>> g;
    vector<ll> level;
    vector<ll> iter;

    MaxFlow(ll n) : n(n), g(n), level(n), iter(n) {}

    // 辺の追加
    ll add_edge(ll from, ll to, Cap cap) {
        ll m = pos.size();
        pos.push_back({from, (ll)g[from].size()});
        ll from_id = g[from].size();
        ll to_id = g[to].size();
        if (from == to) to_id++;
        g[from].push_back({to, cap, to_id});
        g[to].push_back({from, 0, from_id});
        return m;
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

    // 始点 s から終点 t への最大流を計算
    Cap build(ll s, ll t) {
        Cap flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) return flow;
            fill(iter.begin(), iter.end(), 0);
            Cap f = dfs(s, t, numeric_limits<Cap>::max());
            while (f > 0) {
                flow += f;
                f = dfs(s, t, numeric_limits<Cap>::max());
            }
        }
    }

    // i番目に追加した辺の情報を取得
    Edge get_edge(ll i) {
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return Edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
    }

    // 全ての辺の情報を取得
    vector<Edge> get_edges() {
        ll m = pos.size();
        vector<Edge> res(m);
        for (ll i = 0; i < m; i++) {
            res[i] = get_edge(i);
        }
        return res;
    }

    // 始点 s から到達可能な頂点を取得（最小カットのS側）
    vector<bool> get(ll s) {
        vector<bool> visited(n, false);
        queue<ll> q;
        q.push(s);
        visited[s] = true;
        while (!q.empty()) {
            ll v = q.front();
            q.pop();
            for (auto& e : g[v]) {
                if (e.cap > 0 && !visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        return visited;
    }
};