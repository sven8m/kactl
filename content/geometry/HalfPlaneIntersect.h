/**
 * Author: Noé Weeks
 * Date: 2021-10-14
 * License: None
 * Source: Copied from Codechef submission
 * Description: Computes convex polygon which represents
 * intersection of half planes (each half plane is to the
 * left of the segment)
 * Careful with precision !
 * Status: Should work ok
 */
const double EPS = 1e-9;
const double DINF = 1e100;
template <typename T> struct HalfPlane {
  Point<T> p, pq;
  T angle;
  HalfPlane() {}
  HalfPlane(Point<T> _p, Point<T> _q): p(_p), pq(_q - _p), angle(atan2(pq.y, pq.x)) {}
  bool operator<(HalfPlane &b) const { return angle < b.angle; }
  bool out(Point<T> q) { return cross(pq, q - p) < EPS; }
  Point<T> intersect(HalfPlane<T> l) {
    if (abs(cross(pq, l.pq)) < EPS) return Point<T>(DINF, DINF);
    return l.p + l.pq * (cross(p - l.p, pq) / cross(l.pq, pq));
  }
};
// Halfplane to the left of line

template <typename T> vector<Point<T>> intersect(vector<HalfPlane<T>> b) {
  vector<Point<T>> bx = {{DINF, DINF}, {-DINF, DINF}, {-DINF, -DINF}, {DINF, -DINF}};
  for (int i = 0; i < 4; ++i) b.emplace_back(bx[i], bx[(i + 1) % 4]);
  sort(b.begin(), b.end());
  int n = b.size(), q = 1, h = 0;
  vector<HalfPlane<T>> c(b.size() + 10);
  for (int i = 0; i < n; ++i) {
    while (q < h and b[i].out(c[h].intersect(c[h - 1]))) h--;
    while (q < h and b[i].out(c[q].intersect(c[q + 1]))) q++;
    c[++h] = b[i];
    if (q < h and abs(cross(c[h].pq, c[h - 1].pq)) < EPS) {
    	if (dot(c[h].pq, c[h - 1].pq) <= 0) return {};
      h--;
      if (b[i].out(c[h].p))c[h] = b[i];
    }
  }
  while (q < h - 1 and c[q].out(c[h].intersect(c[h - 1]))) h--;
  while (q < h - 1 and c[h].out(c[q].intersect(c[q + 1]))) q++;
  if (h - q <= 1) return {};
  c[h + 1] = c[q];
  vector<Point<T>> s;
  for (int i = q; i < h + 1; ++i) s.push_back(c[i].intersect(c[i + 1]));
  return s;
}
