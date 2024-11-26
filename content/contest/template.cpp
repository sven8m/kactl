#include <bits/stdc++.h>
#define int long long
#define ll long long
#define sz(x) x.size()
#define all(x) x.begin(),x.end()
#define rep(i,a,b) for(int i=a;i<b;i++)
using namespace std;

string to_string(string s) { return s; }
template <typename T> string to_string(T v) {
  bool first = true;
  string res = "[";
  for (const auto &x : v) {
    if (!first) res += ", ";
    first = false;
    res += to_string(x);
  }
  res += "]";
  return res;
}

void dbg_out() { cout << endl; }
template <typename Head, typename... Tail> void dbg_out(Head H, Tail... T) {
  cout << ' ' << to_string(H);
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
