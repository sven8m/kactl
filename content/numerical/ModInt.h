/**
 * Author: Noé Weeks
 * Date: 2022-11-30
 * License: CC0
 * Source: folklore
 * Description: Operators for modular arithmetic.
 */

#pragma once
template <const int32_t MOD> struct ModInt {
  int32_t x;
  ModInt() : x(0) {}
  ModInt(long long u) : x(u % MOD) { if (x < 0) x += MOD; }
  ModInt &operator+=(const ModInt &m) {
    x += m.x;
    if (x >= MOD) x -= MOD;
    return *this;
  }
  ModInt &operator-=(const ModInt &m) {
    x -= m.x;
    if (x < 0LL) x += MOD;
    return *this;
  }
  ModInt &operator*=(const ModInt &m) { x = (1LL * x * m.x) % MOD; return *this; }
  friend ModInt operator-(const ModInt &a) {
    ModInt res(a);
    if (res.x) res.x = MOD - res.x;
    return res;
  }
  friend ModInt operator+(const ModInt &a, const ModInt &b) {
    return ModInt(a) += ModInt(b);
  }
  friend ModInt operator-(const ModInt &a, const ModInt &b) {
    return ModInt(a) -= ModInt(b);
  }
  friend ModInt operator*(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= ModInt(b);
  }

  static long long fp(long long u, long long b) {
    long long res = 1LL;
    for (; b; b /= 2, u = u * u % MOD)
      if (b & 1LL)
        res = (res * u) % MOD;
    return res;
  }

  ModInt fastpow(long long k) { return ModInt(fp(x, k)); }
  ModInt inv() { assert(x); return ModInt(fp(x, MOD - 2));
  }
  ModInt &operator/=(const ModInt &m) { return *this *= ModInt(m).inv(); }
  friend ModInt operator/(const ModInt &a, const ModInt &b) {
    return ModInt(a) *= ModInt(b).inv();
  }
};
