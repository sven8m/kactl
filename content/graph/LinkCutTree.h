struct SplayTree {
  struct Node {
    int ch[2] = {0, 0}, p = 0;
    int self = 0, path = 0;
    int sub = 0, vir = 0;
    bool flip = 0;
  };
  vector<Node> T;
 
  SplayTree(int n) : T(n + 1) {}
 
  void push(int x) {
    if (!x || !T[x].flip) return;
    int l = T[x].ch[0], r = T[x].ch[1];
    T[l].flip ^= 1, T[r].flip ^= 1;
    swap(T[x].ch[0], T[x].ch[1]);
    T[x].flip = 0;
  }
 
  void pull(int x) {
    int l = T[x].ch[0], r = T[x].ch[1];
    push(l); push(r);
    T[x].path = T[l].path + T[x].self + T[r].path;
    T[x].sub = T[x].vir + T[l].sub + T[r].sub + T[x].self;
  }
 
  void set(int x, int d, int y) {
    T[x].ch[d] = y, T[y].p = x;
    pull(x);
  }
 
  void splay(int x) {
    auto dir = [&](int x) {
      int p = T[x].p;
      if (!p) return -1;
      return T[p].ch[0] == x ? 0 : T[p].ch[1] == x ? 1 : -1;
    };
    auto rotate = [&](int x) {
      int y = T[x].p, z = T[y].p, dx = dir(x), dy = dir(y);
      set(y, dx, T[x].ch[!dx]);
      set(x, !dx, y);
      if (~dy) set(z, dy, x);
      T[x].p = z;
    };
    for (push(x); ~dir(x);) {
      int y = T[x].p, z = T[y].p;
      push(z); push(y); push(x);
      int dx = dir(x), dy = dir(y);
      if (~dy) rotate(dx != dy ? x : y);
      rotate(x);
    }
  }
};
 
struct LinkCut : SplayTree {
  LinkCut(int n) : SplayTree(n) {}
 
  int access(int x) {
    int u = x, v = 0;
    for (; u; v = u, u = T[u].p) {
      splay(u);
      int &ov = T[u].ch[1];
      T[u].vir += T[ov].sub;
      T[u].vir -= T[v].sub;
      ov = v; pull(u);
    }
    splay(x);
    return v;
  }
 
  void reroot(int x) {
    access(x);
    T[x].flip ^= 1;
    push(x);
  }
 
  void link(int u, int v) {
    ++u, ++v;
    reroot(u);
    access(v);
    T[v].vir += T[u].sub;
    T[u].p = v;
    pull(v);
  }
 
  void cut(int u, int v) {
    ++u, ++v;
    reroot(u);
    access(v);
    T[v].ch[0] = T[u].p = 0;
    pull(v);
  }
 
  int lca(int u, int v) {
    ++u, ++v;
    if (u == v) return u;
    access(u);
    int ret = access(v);
    return T[u].p ? ret : 0;
  }
 
  // subtree aggregate for u when footed at v
  int subTree(int u, int v) {
    ++u, ++v;
    reroot(v); access(u);
    return T[u].vir + T[u].self;
  }
 
  // puts v at root and path to u is in left child
  // returns path aggregate
  int path(int u, int v)
  {
    ++u, ++v;
    reroot(u);
    access(v);
    return T[v].path;
  }
 
  void update(int u, int v) {
    ++u;
    access(u);
    T[u].self = v;
    pull(u);
  }
};