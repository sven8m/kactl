/**
 * Author: Noé Weeks
 * Date: 2022-11-30
 * License: CC0
 * Source: folklore
 * Description: Evaluates pol at pts[0], ..., pts[N-1]
 * Time: $O(N \log^2 N)$
 */


#pragma once
vector<int> mulT(vector<int> &a, vector<int> b) {
  if (b.empty()) return {};
  int n = b.size();
  reverse(b.begin(), b.end());
  auto prd = conv(a, b);
  if ((int)prd.size() < n) return {};
  return {prd.begin() + n - 1, prd.end()};
}

vector<Mint> multiPoint(vector<int> pol, vector<int> pts) {
  if (pts.empty()) return {};
  int n = max(pol.size(), pts.size());
  vector<vector<Mint>> seg(4 * n);
  vector<Mint> sol(pts.size()); pts.resize(n);
  auto build = [&](auto rec, int node, int deb, int fin) -> void {
    if (deb + 1 == fin) {
      seg[node] = {1, -pts[deb]};
    } else {
      int mid = (deb + fin) / 2;
      rec(rec, 2 * node, deb, mid); rec(rec, 2 * node + 1, mid, fin);
      seg[node] = conv(seg[2 * node], seg[2 * node + 1]);
    }
  };
  build(build, 1, 0, n);
  auto trav = [&](auto rec, int node, int deb, int fin,
                  vector<Mint> cur) -> void {
    if (deb + 1 == fin) {
      if (deb < (int)sol.size())
        sol[deb] = cur[0];
      return;
    }
    int mid = (deb + fin) / 2;
    vector<int> lft = mulT(cur, seg[2 * node + 1]);
    lft.resize(min((int)lft.size(), mid - deb));
    rec(rec, 2 * node, deb, mid, move(lft));
    vector<int> rgt = mulT(cur, seg[2 * node]);
    rgt.resize(min((int)rgt.size(), fin - mid));
    rec(rec, 2 * node + 1, mid, fin, move(rgt));
  };
  trav(trav, 1, 0, n, mulT(pol, inversePowerSeries(seg[1], n)));
  return sol;
}
