/**
 * Author: 300iq
 * Date: 2017-04-20
 * License: CC0
 * Source: own work
 * Description: Container where you can add lines of the form kx+b, and query maximum values at points x.
 * Lines must be added by increasing slope
 * Time: O(\log N)
 * Status: stress-tested
 */

struct cht {
  vector<double> from;
  vector<pair<int, int>> arr;
  vector<pair<int, int>> rem;
  vector<double> remf;
  vector<int> cnt;
  int fst = 0;

  // increasing k
  void add(int k, int b) {
    double x = -1e18;
    cnt.push_back(0);
    while (arr.size() && (x = (double)(b - arr.back().second) /
                              (arr.back().first - k)) < from.back()) {
      ++cnt.back();
      rem.push_back(arr.back());
      remf.push_back(from.back());
      arr.pop_back();
      from.pop_back();
    }
    from.push_back(x);
    arr.emplace_back(k, b);
  }
  // maximum
  double get(double x) {
    if (!from.size())
      return 0;
    int ind = upper_bound(from.begin(), from.end(), x) - from.begin() - 1;
    return arr[ind].first * x + arr[ind].second;
  }

  // TO CHECK
  double getIncreasing(double x) {
    while (fst + 1 < (int)arr.size() and
           arr[fst].first * x + arr[fst].second <
               arr[fst + 1].first * x + arr[fst + 1].second)
      ++fst;
    return arr[fst].first * x + arr[fst].second;
  }

  void undo() {
    arr.pop_back();
    from.pop_back();
    for (int i = 0; i < cnt.back(); ++i) {
      arr.push_back(rem.back()), rem.pop_back();
      from.push_back(remf.back()), remf.pop_back();
    }
    cnt.pop_back();
  }
};
