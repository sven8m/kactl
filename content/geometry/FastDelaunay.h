/**
 * Author: Philippe Legault
 * Date: 2016
 * License: MIT
 * Source: https://github.com/Bathlamos/delaunay-triangulation/
 * Description: Fast Delaunay triangulation.
 * Each circumcircle contains none of the input points.
 * There must be no duplicate points.
 * If all points are on a line, no triangles will be returned.
 * Should work for doubles as well, though there may be precision issues in 'circ'.
 * Returns triangles in order \{t[0][0], t[0][1], t[0][2], t[1][0], \dots\}, all counter-clockwise.
 * Time: O(n \log n)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"
struct quad_edge {
  int o = -1; // origin of the arc
  quad_edge *onext, *rot;
  bool mark = false;
  quad_edge() {}
  quad_edge(int o) : o(o) {}
  int d() { return sym()->o; } // destination of the arc
  quad_edge *sym() { return rot->rot; }
  quad_edge *oprev() { return rot->onext->rot; }
  quad_edge *lnext() { return sym()->oprev(); }
  static quad_edge *make_sphere(int a, int b) {
    array<quad_edge *, 4> q{
        {new quad_edge{a}, new quad_edge{}, new quad_edge{b}, new quad_edge{}}};
    for (auto i = 0; i < 4; ++i)
      q[i]->onext = q[-i & 3], q[i]->rot = q[i + 1 & 3];
    return q[0];
  }
  static void splice(quad_edge *a, quad_edge *b) {
    swap(a->onext->rot->onext, b->onext->rot->onext);
    swap(a->onext, b->onext);
  }
  static quad_edge *connect(quad_edge *a, quad_edge *b) {
    quad_edge *q = make_sphere(a->d(), b->o);
    splice(q, a->lnext()), splice(q->sym(), b);
    return q;
  }
};

template <class T, class T_large, class F1, class F2>
bool delaunay_triangulation(const vector<Point<T>> &a, F1 process_outer_face,
                            F2 process_triangles) {
  vector<int> ind(a.size());
  iota(ind.begin(), ind.end(), 0);
  sort(ind.begin(), ind.end(), [&](int i, int j) { return a[i] < a[j]; });
  ind.erase(unique(ind.begin(), ind.end(),
                   [&](int i, int j) { return a[i] == a[j]; }),
            ind.end());
  int n = (int)ind.size();
  if (n < 2)
    return {};
  auto circular = [&](Point<T> p, Point<T> a, Point<T> b, Point<T> c) {
    a = a - p, b = b - p, c = c - p;
    return ((T_large)a.dist2() * b.cross(c) + (T_large)b.dist2() * c.cross(a) +
            (T_large)c.dist2() * a.cross(b)) *
               (a.cross(b, c) > 0 ? 1 : -1) >
           0;
  };
  auto recurse = [&](auto self, int l, int r) -> array<quad_edge *, 2> {
    if (r - l <= 3) {
      quad_edge *p = quad_edge::make_sphere(ind[l], ind[l + 1]);
      if (r - l == 2)
        return {p, p->sym()};
      quad_edge *q = quad_edge::make_sphere(ind[l + 1], ind[l + 2]);
      quad_edge::splice(p->sym(), q);
      auto side = a[ind[l]].cross(a[ind[l + 1]], a[ind[l + 2]]);
      quad_edge *c = side ? quad_edge::connect(q, p) : NULL;
      return {side < 0 ? c->sym() : p, side < 0 ? c : q->sym()};
    }
    int m = l + (r - l >> 1);
    auto [ra, A] = self(self, l, m);
    auto [B, rb] = self(self, m, r);
    while (a[B->o].cross(a[A->d()], a[A->o]) < 0 && (A = A->lnext()) ||
           a[A->o].cross(a[B->d()], a[B->o]) > 0 && (B = B->sym()->onext))
      ;
    quad_edge *base = quad_edge::connect(B->sym(), A);
    if (A->o == ra->o)
      ra = base->sym();
    if (B->o == rb->o)
      rb = base;
#define valid(e) (a[e->d()].cross(a[base->d()], a[base->o]) > 0)
#define DEL(e, init, dir) \
  quad_edge *e = init->dir; \
  if (valid(e)) \
    while (circular(a[e->dir->d()], a[base->d()], a[base->o], a[e->d()])) { \
      quad_edge *t = e->dir; \
      quad_edge::splice(e, e->oprev());\
      quad_edge::splice(e->sym(), e->sym()->oprev()); \
      delete e->rot->rot->rot; \
      delete e->rot->rot; \
      delete e->rot; \
      delete e; \
      e = t; \
    }
    while (true) {
      DEL(LC, base->sym(), onext);
      DEL(RC, base, oprev());
      if (!valid(LC) && !valid(RC))
        break;
      if (!valid(LC) ||
          valid(RC) && circular(a[RC->d()], a[RC->o], a[LC->d()], a[LC->o]))
        base = quad_edge::connect(RC, base->sym());
      else
        base = quad_edge::connect(base->sym(), LC->sym());
    }
    return {ra, rb};
  };
  auto e = recurse(recurse, 0, n)[0];
  vector<quad_edge *> q = {e}, rem;
  while (a[e->onext->d()].cross(a[e->d()], a[e->o]) < 0)
    e = e->onext;
  vector<int> face;
  face.reserve(n);
  bool colinear = false;
#define ADD \
  { \
    quad_edge *c = e; \
    face.clear(); \
    do {  \
      c->mark = true; \
      face.push_back(c->o);  \
      q.push_back(c->sym()); \
      rem.push_back(c); \
      c = c->lnext();  \
    } while (c != e); \
  }
  ADD;
  process_outer_face(face);
  for (auto qi = 0; qi < (int)q.size(); ++qi) {
    if (!(e = q[qi])->mark) {
      ADD;
      colinear = false;
      process_triangles({face[0], face[1], face[2]});
    }
  }
  for (auto e : rem)
    delete e->rot, delete e;
  return !colinear;
}
