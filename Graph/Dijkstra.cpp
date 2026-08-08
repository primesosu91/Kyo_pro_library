// 単一始点最短経路を求めるダイクストラ法
//
// example:
//
// yosupo judge - Shortest Path
// https://judge.yosupo.jp/submission/391376
// 単一始点最短経路
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll>
struct Edge {
    ll from;
    ll to;
    T cost;
    ll id;
};

template <typename T = ll>
struct Dijkstra {
    ll n;
    T inf;
    ll edge_count;
    vector<vector<Edge<T>>> g;
    vector<T> dist;
    vector<Edge<T>> pre;

    // 初期化
    Dijkstra(ll n) : n(n), edge_count(0), g(n), dist(n), pre(n) {
        inf = numeric_limits<T>::max() / 2;
    }

    // 辺の追加
    void add_edge(ll from, ll to, T cost, ll id = -1) {
        if (id == -1) {
            id = edge_count;
        }
        edge_count++;
        g[from].push_back({from, to, cost, id});
    }

    // 始点 s からの最短距離を計算
    void build(ll s) {
        for (ll i = 0; i < n; i++) {
            dist[i] = inf;
            pre[i] = {-1, -1, 0, -1};
        }

        using P = pair<T, ll>;
        priority_queue<P, vector<P>, greater<P>> pq;
        dist[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (dist[u] < d) {
                continue;
            }

            for (const auto& edge : g[u]) {
                if (dist[edge.to] > dist[u] + edge.cost) {
                    dist[edge.to] = dist[u] + edge.cost;
                    pre[edge.to] = edge;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
    }

    // 頂点 t への最短距離を取得 (到達不可なら -1)
    T get(ll t) {
        if (dist[t] == inf) {
            return (T)-1;
        }
        return dist[t];
    }

    // 始点から頂点 t までの通過辺配列を復元 (到達不可なら空配列)
    vector<Edge<T>> get_path(ll t) {
        vector<Edge<T>> path;
        if (dist[t] == inf) {
            return path;
        }

        ll curr = t;
        while (pre[curr].from != -1) {
            path.push_back(pre[curr]);
            curr = pre[curr].from;
        }

        reverse(path.begin(), path.end());
        return path;
    }
};