/**
 * Author: Noé Weeks
 * Date: 2022-11-30
 * License: CC0
 * Source: folklore
 * Description: Inverse of FPS $\pmod{X^n}$
 * Time: $O(N \log N)$
 */

#pragma once
vector<int> inversePowerSeries(vector<int> p, int n) {
  vector<int> inv(1, modpow(p[0], MOD - 2));
  for (int curSz = 2; curSz < 2 * n; curSz *= 2) {
    vector<int> cur(2 * curSz);
    for (int i = 0; i < curSz; ++i)
      cur[i] = i < (int)p.size() ? p[i] : 0;
    inv.resize(2 * curSz);
    ntt(inv), ntt(cur);
    int invSz = modpow(2 * curSz, MOD - 2);
    for (int i = 0; i < 2 * curSz; ++i)
      inv[i] = inv[i] * (2 - cur[i] * inv[i] % MOD + MOD) % MOD * invSz % MOD;
    reverse(inv.begin() + 1, inv.begin() + 2 * curSz);
    ntt(inv);
    fill(inv.begin() + curSz, inv.begin() + 2 * curSz, 0);
  }
  return {inv.begin(), inv.begin() + n};
}
