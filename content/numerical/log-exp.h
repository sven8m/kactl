/**
 * Author: Noé Weeks
 * Date: 2022-11-30
 * License: CC0
 * Source: folklore
 * Description: Exp/Log of FPS $\pmod{X^n}$
 * Time: $O(N \log N)$
 */

#pragma once

// n should be power of 2
// a is modified
vector<int> logPowerSeries(vector<int> &a, int n) {
  a.resize(n);
  a[0] = 1;
  auto b = inversePowerSeries(a, n); 
  for (int i = 0; i < n - 1; ++i)
    a[i] = (i + 1) * a[i + 1] % mod;
  a = conv(a, b); 
  for (int i = n - 1; i >= 1; --i)
    a[i] = a[i - 1] * modpow(i, mod - 2) % mod;
  a[0] = 0;
  return a;
}

vector<int> expPowerSeries(vector<int> &a, int n) {
  vector<int> r(1, 1); 
  a.resize(n);
  for (int m = 2; m <= n; m *= 2) {
    vector<int> x = r;
    x = logPowerSeries(x, m); 
    for (int i = 0; i < m; ++i)
      x[i] = (a[i] - x[i] + mod) % mod;
    x[0] = (x[0] + 1) % mod;
    r.resize(m);
    x.resize(m);
    r = conv(r, x); 
    r.resize(m);
  }
  return r;
}
