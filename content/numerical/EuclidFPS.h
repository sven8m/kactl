/**
 * Author: Noé Weeks
 * Date: 2022-11-30
 * License: CC0
 * Source: folklore
 * Description: Euclid division of FPS
 * Time: $O(N \log N)$
 */

#pragma once
pair<vector<int>, vector<int>> euclidFPS(vector<int> A, vector<int> B) {
  while (!B.empty() and !B.back()) B.pop_back();
  int n = A.size(), m = B.size();
  if (n < m) return pair(vector<int>{}, A);
  reverse(A.begin(), A.end()), reverse(B.begin(), B.end());
  auto invB = inversePowerSeries(B, n - m + 1);
  auto quotient = conv(A, invB);
  quotient.resize(n - m + 1);
  reverse(quotient.begin(), quotient.end());
  while (!quotient.empty() and quotient.back().x == 0) quotient.pop_back();
  reverse(A.begin(), A.end()), reverse(B.begin(), B.end());
  auto R = conv(B, quotient);
  for (int i = 0; i < (int)R.size(); ++i) {
    A[i] -= R[i];
    if (A[i] < 0) A[i] += MOD;
  }
  while (!A.empty() and !A.back()) A.pop_back();
  return pair(quotient, A);
}