/**
 * Author: Adamant
 * Date: 2022-11-17
 * License: CC0
 * Source: https://codeforces.com/blog/entry/103785?#comment-922682
 * Description: Implements multiplication and division in Nimber field where addition = xor. 
 * Can be carried for any unsigned 64 bit integer
 * Status: Tested
 * Time: decent constant factor
 */

using Nimber = uint64_t;
bool log_computed = false;
const int small = 1 << 16;
uint16_t pw[small], lg[small];

Nimber mul(Nimber a, Nimber b, int half = 32) {
  if (a <= 1 or b <= 1) return a * b;
  if (log_computed and a < small and b < small) {
    int t = (int)lg[a] + lg[b];
    return pw[t >= small - 1 ? t - (small - 1) : t];
  }
  auto mask = (1ULL << half) - 1;
  auto [a0, a1] = make_pair(a & mask, a >> half);
  auto [b0, b1] = make_pair(b & mask, b >> half);
  auto A = mul(a0, b0, half / 2);
  auto C = mul(a1, b1, half / 2);
  auto B = mul(a0 ^ a1, b0 ^ b1, half / 2) ^ A ^ C;
  B = (B << half);
  C = (C << half) ^ mul(C, 1ULL << (half - 1), half / 2);
  return A ^ B ^ C;
}

Nimber fastpow(Nimber a, int b) {
  Nimber ret = 1;
  for (; b; b /= 2, a = mul(a, a)) 
    if (b % 2) ret = mul(ret, a);
  return ret;
}

Nimber inv(Nimber x) { return fastpow(x, -2); }

void initNimber() { // CALL THIS
  pw[0] = 1;
  lg[1] = 0;
  uint16_t base = -1;
  for (int i = 1; i < small - 1; ++i)
    pw[i] = mul(pw[i - 1], base), lg[pw[i]] = i;
  log_computed = true;
}