// 有向・無向両対応のサイクル検出と復元
//
// example:
//
// yosupo judge - Cycle Detection (Undirected)
// https://judge.yosupo.jp/submission/391314
// 無向グラフのサイクル判定
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge {
    ll to;
    ll id;
};

struct CycleResult {
    bool exists = false;
    vector<ll> vertices;
    vector<ll> edges;
};

struct Cycle {
    ll n;
    bool directed;
    vector<vector<Edge>> g;
    ll edge_count;

    Cycle(ll n, bool directed = true) : n(n), directed(directed), g(n), edge_count(0) {}

    // 辺を追加し、追加した辺のIDを返す
    ll add_edge(ll from, ll to) {
        g[from].push_back({to, edge_count});
        // 無向グラフで自己ループでない場合のみ逆辺を追加
        if (!directed && from != to) {
            g[to].push_back({from, edge_count});
        }
        return edge_count++;
    }

    // サイクル検出と復元を実行
    CycleResult build() {
        // 0: 未訪問, 1: 訪問中, 2: 訪問完了
        vector<ll> state(n, 0);
        vector<ll> pre_vertex(n, -1);
        vector<ll> pre_edge(n, -1);
        
        // DFSクロージャ。サイクルを検出した場合、その合流点（起点の頂点）を返す
        auto dfs = [&](auto& dfs, ll u, ll p_edge) -> ll {
            state[u] = 1;
            
            for (const auto& e : g[u]) {
                // 無向グラフの場合、直前に通った辺を逆走しない
                if (!directed && e.id == p_edge) continue;
                
                // 訪問中の頂点に到達 = サイクル検出
                if (state[e.to] == 1) {
                    pre_vertex[e.to] = u;
                    pre_edge[e.to] = e.id;
                    return e.to;
                }
                
                // 未訪問の頂点へ進む
                if (state[e.to] == 0) {
                    pre_vertex[e.to] = u;
                    pre_edge[e.to] = e.id;
                    ll res = dfs(dfs, e.to, e.id);
                    if (res != -1) return res;
                }
            }
            
            state[u] = 2;
            return -1;
        };
        
        CycleResult res;
        
        // 非連結グラフに対応するため全頂点を始点候補とする
        for (ll i = 0; i < n; i++) {
            if (state[i] == 0) {
                ll cycle_start = dfs(dfs, i, -1);
                
                if (cycle_start != -1) {
                    res.exists = true;
                    ll curr = cycle_start;
                    
                    // 経路復元 (合流点から逆順にたどる)
                    do {
                        res.edges.push_back(pre_edge[curr]);
                        curr = pre_vertex[curr];
                        res.vertices.push_back(curr);
                    } while (curr != cycle_start);
                    
                    // 逆順に追加されているため、正しい順序に反転
                    reverse(res.vertices.begin(), res.vertices.end());
                    reverse(res.edges.begin(), res.edges.end());
                    
                    return res;
                }
            }
        }
        
        return res;
    }
};