/**
 * Author: Bicsi 
 * Date: 2022-12-13
 * License: CC0
 * Description: Solves min cost circulation in network, gives both flow values and dual.
 * To get flow of edge look at capacity of back edge.
 * To solve Min Cost Max Flow, add edge from tap to source of infinite capacity and 
 * cost -INF where $INF = 1 + \sum_e abs(weight_e)$
 * To recover max flow, look at flow on this back edge and don't forget to update price
 * To solve flow with demands on vertices, add an extra node and use INF cost again
 * to enforce circulation. Dual values obtained are the same as if you didn't do transformation
 * Time: Should be fast $O(M^2)$ worst case, add random shuffle of edges (permute indices)
 * if time might be an issue. Should be faster than MCMF.
 * DANGER : dual variables can cause problems if not in same connected component.
 */
#pragma once

template <class Flow, class Cost> struct NetworkSimplex {
  struct Edge {
    int nxt, to;
    Flow cap;
    Cost cost;
  };
  vector<Edge> edges;
  vector<int> head, fa, fe, mark, cyc;
  vector<Cost> dual;
  int ti;

  NetworkSimplex(int n)
      : head(n, 0), fa(n), fe(n), mark(n), cyc(n + 1), dual(n), ti(0) {
    edges.push_back({0, 0, 0, 0});
    edges.push_back({0, 0, 0, 0});
  }

  int addEdge(int u, int v, Flow cap, Cost cost) {
    assert(edges.size() % 2 == 0);
    int e = edges.size();
    edges.push_back({head[u], v, cap, cost});
    head[u] = e;
    edges.push_back({head[v], u, 0, -cost});
    head[v] = e + 1;
    return e;
  }

  void initTree(int x) {
    mark[x] = 1;
    for (int i = head[x]; i; i = edges[i].nxt) {
      int v = edges[i].to;
      if (!mark[v] and edges[i].cap) {
        fa[v] = x, fe[v] = i;
        initTree(v);
      }
    }
  }

  int phi(int x) {
    if (mark[x] == ti) return dual[x];
    return mark[x] = ti, dual[x] = phi(fa[x]) - edges[fe[x]].cost;
  }

  void pushFlow(int e, Cost &cost) {
    int pen = edges[e ^ 1].to, lca = edges[e].to;
    ti++;
    while (pen) mark[pen] = ti, pen = fa[pen];
    while (mark[lca] != ti) mark[lca] = ti, lca = fa[lca];

    int e2 = 0, f = edges[e].cap, path = 2, clen = 0;
    for (int i = edges[e ^ 1].to; i != lca; i = fa[i]) {
      cyc[++clen] = fe[i];
      if (edges[fe[i]].cap < f)
        f = edges[fe[e2 = i] ^ (path = 0)].cap;
    }
    for (int i = edges[e].to; i != lca; i = fa[i]) {
      cyc[++clen] = fe[i] ^ 1;
      if (edges[fe[i] ^ 1].cap <= f)
        f = edges[fe[e2 = i] ^ (path = 1)].cap;
    }
    cyc[++clen] = e;

    for (int i = 1; i <= clen; ++i) {
      edges[cyc[i]].cap -= f, edges[cyc[i] ^ 1].cap += f;
      cost += edges[cyc[i]].cost * f;
    }
    if (path == 2)  return;

    int laste = e ^ path, last = edges[laste].to, cur = edges[laste ^ 1].to;
    while (last != e2) {
      mark[cur]--;
      laste ^= 1;
      swap(laste, fe[cur]);
      swap(last, fa[cur]); swap(last, cur);
    }
  }

  Cost compute() {
    Cost cost = 0;
    initTree(0);
    mark[0] = ti = 2, fa[0] = cost = 0;
    int ncnt = edges.size() - 1;
    for (int i = 2, pre = ncnt; i != pre; i = i == ncnt ? 2 : i + 1)
      if (edges[i].cap and edges[i].cost < phi(edges[i ^ 1].to) - phi(edges[i].to))
        pushFlow(pre = i, cost);
    ++ti;
    for (int u = 0; u < (int)dual.size(); ++u)
      phi(u);
    return cost;
  }
};
