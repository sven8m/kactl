/**
 * Author: TWal
 * Date: 2010-04-04
 * License: CC0
 * Source: folklore
 * Description: Builds dominator tree of digraph rooted at source
 * dom[u] = children of u in dominator u, where u is ancestor of v iff all
 * paths from source to v contain u
 * Time: $O(E\log V)$
 * Status: Unknown testing
 */
#pragma once

struct DominatorTree {
  int n, sz = 0;
  vi dfn, pre, pt, semi, dsu, idom, best;
  vvi dom;

  DominatorTree(const vvi &g, lli source)
      : n(g.size()), dfn(n, -1), pre(n), pt(n), semi(n), dsu(n), idom(n),
        best(n), dom(n) {
    vvi ginv(n);
    FOR(i, n) {
      for (lli j : g[i])
        ginv[j].pb(i);
      dsu[i] = best[i] = semi[i] = i;
    }
    dfs(source, g);
    vvi mydom(n);
    tarjan(ginv, mydom);
    FOR(i, sz) for (lli d : mydom[i]) dom[pt[i]].pb(pt[d]);
  }

  lli get(lli x) {
    if (x == dsu[x])
      return x;
    lli y = get(dsu[x]);
    if (semi[best[x]] > semi[best[dsu[x]]])
      best[x] = best[dsu[x]];
    return dsu[x] = y;
  }

  void dfs(lli u, const vvi &succ) {
    dfn[u] = sz;
    pt[sz++] = u;
    for (lli v : succ[u])
      if (dfn[v] < 0) {
        dfs(v, succ);
        pre[dfn[v]] = dfn[u];
      }
  }

  void tarjan(const vvi &pred, vvi &dom) {
    FORD(j, 1, sz) {
      lli u = pt[j];
      for (lli tv : pred[u])
        if (dfn[tv] >= 0) {
          lli v = dfn[tv];
          get(v);
          if (semi[best[v]] < semi[j])
            semi[j] = semi[best[v]];
        }
      dom[semi[j]].push_back(j);
      lli x = dsu[j] = pre[j];
      for (lli z : dom[x]) {
        get(z);
        if (semi[best[z]] < x)
          idom[z] = best[z];
        else
          idom[z] = x;
      }
      dom[x].clear();
    }
    FORU(i, 1, sz) {
      if (semi[i] != idom[i])
        idom[i] = idom[idom[i]];
      dom[idom[i]].push_back(i);
    }
  }
};
