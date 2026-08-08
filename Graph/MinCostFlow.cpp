// 最小費用流
//
// example:
// ABC 407 G - Domino Covering SUM (3D)
// https://atcoder.jp/contests/abc407/submissions/78185382
// ドミノで覆われない数の最大化
//

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class Cap = ll, class Cost = ll>
struct MinCostFlow {
    struct _Edge {
        ll to;
        Cap cap;
        Cost cost;
        ll rev;
    };

    struct Edge {
        ll from;
        ll to;
        Cap cap;
        Cap flow;
        Cost cost;
    };

    ll n;
    vector<pair<ll, ll>> pos;
    vector<vector<_Edge>> g;

    MinCostFlow(ll n) : n(n), g(n) {}

    // 辺の追加
    ll add_edge(ll from, ll to, Cap cap, Cost cost) {
        ll m = pos.size();
        pos.push_back({from, (ll)g[from].size()});
        ll from_id = g[from].size();
        ll to_id = g[to].size();
        if (from == to) to_id++;
        g[from].push_back({to, cap, cost, to_id});
        g[to].push_back({from, 0, -cost, from_id});
        return m;
    }

    // 流量 flow_limit までの流量と費用の変化点（折れ線）を取得
    vector<pair<Cap, Cost>> slope(ll s, ll t, Cap flow_limit) {
        vector<pair<Cap, Cost>> result;
        result.push_back({0, 0});
        Cap flow = 0;
        Cost cost = 0, prev_cost_per_flow = -1;
        vector<Cost> dual(n, 0), dist(n);
        vector<ll> pre(n), pre_edge(n);

        while (flow < flow_limit) {
            fill(dist.begin(), dist.end(), numeric_limits<Cost>::max());
            fill(pre.begin(), pre.end(), -1);
            fill(pre_edge.begin(), pre_edge.end(), -1);

            // ポテンシャル付きダイクストラ法
            priority_queue<pair<Cost, ll>, vector<pair<Cost, ll>>, greater<pair<Cost, ll>>> pq;
            dist[s] = 0;
            pq.push({0, s});
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();
                if (dist[v] < d) continue;
                for (ll i = 0; i < (ll)g[v].size(); i++) {
                    auto& e = g[v][i];
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + dual[v] - dual[e.to]) {
                        dist[e.to] = dist[v] + e.cost + dual[v] - dual[e.to];
                        pre[e.to] = v;
                        pre_edge[e.to] = i;
                        pq.push({dist[e.to], e.to});
                    }
                }
            }
            if (dist[t] == numeric_limits<Cost>::max()) break;

            for (ll v = 0; v < n; v++) {
                if (dist[v] != numeric_limits<Cost>::max()) dual[v] += dist[v];
            }

            Cap d = flow_limit - flow;
            for (ll v = t; v != s; v = pre[v]) {
                d = min(d, g[pre[v]][pre_edge[v]].cap);
            }
            for (ll v = t; v != s; v = pre[v]) {
                auto& e = g[pre[v]][pre_edge[v]];
                e.cap -= d;
                g[v][e.rev].cap += d;
            }

            Cost d_cost = dual[t];
            flow += d;
            cost += d * d_cost;
            if (prev_cost_per_flow == d_cost) {
                result.pop_back();
            }
            result.push_back({flow, cost});
            prev_cost_per_flow = d_cost;
        }
        return result;
    }

    // 流せるだけ流した場合の流量と費用の変化点を取得
    vector<pair<Cap, Cost>> slope(ll s, ll t) {
        return slope(s, t, numeric_limits<Cap>::max());
    }

    // 流量 flow を流す際の最小費用（不可能なら -1）
    Cost build(ll s, ll t, Cap flow) {
        auto res = slope(s, t, flow);
        if (res.back().first < flow) return -1;
        return res.back().second;
    }

    // 流せるだけ流した場合の最大流量と最小費用
    pair<Cap, Cost> build(ll s, ll t) {
        auto res = slope(s, t);
        return res.back();
    }

    // i番目に追加した辺の情報を取得
    Edge get_edge(ll i) {
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return Edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap, _e.cost};
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
};