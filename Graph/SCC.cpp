// 強連結成分分解 (SCC) - 有向グラフを強連結成分に分解し、トポロジカルソートされたDAGを構築する
//
// example:
//
// yosupo judge - Strongly Connected Components
// https://judge.yosupo.jp/submission/391378
// 連結成分の頂点列挙
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge {
    ll to;
};

struct SCC {
    ll n;
    vector<vector<Edge>> g;
    vector<vector<Edge>> rev_g;
    vector<ll> comp;
    vector<vector<ll>> groups;
    
    // 探索用の内部変数
    vector<ll> order;
    vector<bool> used;

    SCC(ll _n) : n(_n), g(_n), rev_g(_n), comp(_n, -1), used(_n, false) {}

    // 辺を追加 (from -> to)
    void add_edge(ll from, ll to) {
        g[from].push_back({to});
        rev_g[to].push_back({from});
    }

    // 1回目のDFS：帰りがけ順に頂点を記録
    void dfs(ll v) {
        used[v] = true;
        for (auto e : g[v]) {
            if (!used[e.to]) dfs(e.to);
        }
        order.push_back(v);
    }

    // 2回目のDFS：逆辺を辿って強連結成分を構築
    void rdfs(ll v, ll k) {
        used[v] = true;
        comp[v] = k;
        groups[k].push_back(v);
        for (auto e : rev_g[v]) {
            if (!used[e.to]) rdfs(e.to, k);
        }
    }

    // SCCを実行するメイン処理
    void build() {
        for (ll i = 0; i < n; i++) {
            if (!used[i]) dfs(i);
        }
        
        fill(used.begin(), used.end(), false);
        ll k = 0;
        
        for (ll i = (ll)order.size() - 1; i >= 0; i--) {
            if (!used[order[i]]) {
                groups.push_back(vector<ll>());
                rdfs(order[i], k);
                k++;
            }
        }
    }

    // 強連結成分数（グループ数）を返す
    ll size() {
        return groups.size();
    }

    // 指定したグループ k に含まれる頂点リストを返す
    vector<ll> get_group(ll k) {
        return groups[k];
    }

    // 全グループの頂点リスト一覧を返す
    vector<vector<ll>> get_groups() {
        return groups;
    }

    // 元の頂点 v が属するグループIDを返す
    ll get_id(ll v) {
        return comp[v];
    }

    // 縮約後のDAG（隣接リスト）を取得する
    vector<vector<Edge>> get_dag() {
        ll k = groups.size();
        vector<vector<Edge>> dag(k);
        
        // 異なるグループ間の辺を張る
        for (ll u = 0; u < n; u++) {
            for (auto e : g[u]) {
                ll x = comp[u];
                ll y = comp[e.to];
                if (x != y) {
                    dag[x].push_back({y});
                }
            }
        }
        
        // 重複する辺を削除して一意にする
        for (ll i = 0; i < k; i++) {
            sort(dag[i].begin(), dag[i].end(), [](const Edge& a, const Edge& b) {
                return a.to < b.to;
            });
            dag[i].erase(unique(dag[i].begin(), dag[i].end(), [](const Edge& a, const Edge& b) {
                return a.to == b.to;
            }), dag[i].end());
        }
        
        return dag;
    }
};