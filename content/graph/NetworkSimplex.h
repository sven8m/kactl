/**
 * Author: Bicsi 
 * Date: 2022-12-13
 * License: CC0
 * Description: Solves min cost circulation in network, gives both flow values and dual.
 * To solve Min Cost Max Flow, add edge from tap to source of infinite capacity and 
 * cost -INF where $INF = 1 + \sum_e abs(weight_e)$
 * To recover max flow, look at flow on this back edge and don't forget to update price
 * To solve flow with demands on vertices, add an extra node and use INF cost again
 * to enforce circulation. Dual values obtained are the same as if you didn't do transformation
 * Time: Should be fast $O(M^2)$ worst case, add random shuffle of edges (permute indices)
 * if time might be an issue. Should be faster than MCMF
 */
#pragma once

template <class Flow, class Cost> struct NetworkSimplex {
  struct Edge {
    int32_t from, to;
    Cost cost; Flow cap, flow = 0;
  };

  int n;
  vector<int32_t> pei, depth;
  vector<Cost> dual; vector<Edge> E;
  vector<set<int32_t>> tree;

  NetworkSimplex(int _n)
      : n(_n), pei(n + 1, -1), depth(n + 1, 0), 
      dual(n + 1, 0), tree(n + 1) {}

  int addEdge(int from, int to, Flow cap, Cost cost) {
    E.push_back({from, to, cost, cap});
    E.push_back({to, from, -cost, Flow(0)});
    return E.size() - 2;
  }

  void dfs(int node) {
    for (auto ei : tree[node]) {
      if (ei == pei[node])
        continue;
      int vec = E[ei].to;
      dual[vec] = dual[node] + E[ei].cost;
      pei[vec] = (ei ^ 1);
      depth[vec] = 1 + depth[node];
      dfs(vec);
    }
  }

  template <typename F> void walk(int ei, F &&f) {
    f(ei);
    int from = E[ei].from, to = E[ei].to;
    while (from != to) {
      if (depth[from] > depth[to])
        f(pei[from] ^ 1), from = E[pei[from]].to;
      else
        f(pei[to]), to = E[pei[to]].to;
    }
  }

  Cost compute() {
    for (int i = 0; i < n; ++i) {
      int ei = addEdge(n, i, 0, 0);
      tree[n].insert(ei);
      tree[i].insert(ei ^ 1);
    }
    Cost answer = 0, cost; Flow flow;
    int ein, eout, ptr = 0;
    const int B = n / 3 + 1;
    for (int z = 0; z < (int)E.size() / B + 1; ++z) {
      if (!z) dfs(n);
      pair<Cost, int> pin(0, -1);
      for (int t = 0; t < B; ++t, (++ptr) %= E.size()) {
        auto &e = E[ptr];
        if (e.flow < e.cap) pin = min(pin, make_pair(dual[e.from] + e.cost - dual[e.to], ptr));
      }
      tie(cost, ein) = pin;
      if (cost == 0) continue;
      pair<int, int> pout = {E[ein].cap - E[ein].flow, ein};
      walk(ein, [&](int ei) {
        pout = min(pout, make_pair(E[ei].cap - E[ei].flow, ei));
      });
      tie(flow, eout) = pout;
      walk(ein, [&](int ei) { E[ei].flow += flow, E[ei ^ 1].flow -= flow; });
      tree[E[ein].from].insert(ein); 
      tree[E[ein].to].insert(ein ^ 1);
      tree[E[eout].from].erase(eout); 
      tree[E[eout].to].erase(eout ^ 1);
      answer += Cost(flow) * cost; z = -1;
    }
    return answer;
  }
};
