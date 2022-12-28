#include <bits/stdc++.h>
#define int long long
using namespace std;

namespace std {
template <typename T> ostream &operator<<(ostream &out, const vector<T> &vec) {
  out << "[";
  for (int i = 0; i < (int)vec.size(); ++i) {
    out << vec[i];
    if (i + 1 < (int)vec.size())
      out << ", ";
  }
  return out << "]";
}
} // namespace std

void dbg_out() { cout << endl; }
template <typename Head, typename... Tail> void dbg_out(Head H, Tail... T) {
  cout << ' ' << H;
  dbg_out(T...);
}

#ifdef DEBUG
#define dbg(...) cout << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif

signed main(void) {
  ios_base::sync_with_stdio(false);
  cin.tie(0);
}
