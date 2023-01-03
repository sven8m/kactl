/**
 * Author: Simon Lindholm
 * Date: 2015-03-15
 * License: CC0
 * Source: own work
 * Description: Various self-explanatory methods for string hashing.
 * Use on Codeforces, which lacks 64-bit support and where solutions can be hacked.
 * Status: stress-tested
 */
#pragma once
using ull = uint64_t;
constexpr ull mod = (1ull << 61) - 1;
ull modmul(ull a, ull b) {
  ull l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32; 
  ull l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2; 
  ull ret = (l & mod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
  ret = (ret & mod) + (ret >> 61);
  ret = (ret & mod) + (ret >> 61);
  return ret - 1;
}
const int base = randint(1e11, 3e11); // pick random baseb