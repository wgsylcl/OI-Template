#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
inline void chkmx(int &a, int b) { a = a < b ? b : a; }
inline void chkmn(int &a, int b) { a = a > b ? b : a; }
inline void chkmx(ll &a, ll b) { a = a < b ? b : a; }
inline void chkmn(ll &a, ll b) { a = a > b ? b : a; }
const int MAXN = 5e5 + 10;
int n, deep[MAXN], fa[MAXN][24], qq, lsh[MAXN], rsh[MAXN], d[MAXN], stk[MAXN], top;
vector<int> e[MAXN];
void dfs(int u, int f)
{
    deep[u] = deep[f] + 1;
    fa[u][0] = f;
    for (int i = 1; i <= log2(deep[u]); i++)
        fa[u][i] = fa[fa[u][i - 1]][i - 1];
    for (int v : e[u])
        if (v != f)
            dfs(v, u);
}
int getlca(int x, int y)
{
    if (deep[x] < deep[y])
        swap(x, y);
    int d = deep[x] - deep[y];
    for (int i = 0; i <= log2(d); i++)
        if (d & (1 << i))
            x = fa[x][i];
    if (x == y)
        return x;
    for (int i = log2(deep[x]); i + 1; i--)
        if (fa[x][i] != fa[y][i])
            x = fa[x][i], y = fa[y][i];
    return fa[x][0];
}
struct Line
{
    int x, y, v;
} lines[MAXN];
struct node
{
    int l, r, k, pos;
} q[MAXN];
int ans[MAXN];
struct SGT
{
#define mid ((l + r) >> 1)
#define ls ((x) << 1)
#define rs ((x) << 1 | 1)
    int tr[MAXN << 2];
    void clear() { memset(tr, -0x3f, sizeof(tr)); }
    void build(int x, int l, int r)
    {
        if (l == r)
        {
            tr[x] = deep[l];
            return;
        }
        build(ls, l, mid);
        build(rs, mid + 1, r);
        tr[x] = max(tr[ls], tr[rs]);
    }
    void update(int x, int l, int r, int p, int val)
    {
        chkmx(tr[x], val);
        if (l == p && p == r)
            return;
        if (p <= mid)
            update(ls, l, mid, p, val);
        else
            update(rs, mid + 1, r, p, val);
    }
    int query(int x, int l, int r, int pl, int pr)
    {
        if (pl <= l && r <= pr)
            return tr[x];
        int res = -1e9;
        if (pl <= mid)
            chkmx(res, query(ls, l, mid, pl, pr));
        if (mid < pr)
            chkmx(res, query(rs, mid + 1, r, pl, pr));
        return res;
    }
} sgt;
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n;
    for (int u, v, i = 1; i < n; i++)
        cin >> u >> v, e[u].push_back(v), e[v].push_back(u);
    dfs(1, 0);
    for (int i = 1; i < n; i++)
        d[i] = deep[getlca(i, i + 1)];
    for (int i = 1; i < n; i++)
    {
        while (top && d[stk[top]] >= d[i])
            top--;
        lsh[i] = top ? stk[top] + 1 : 1;
        stk[++top] = i;
    }
    top = 0;
    for (int i = n - 1; i; i--)
    {
        while (top && d[stk[top]] >= d[i])
            top--;
        rsh[i] = top ? stk[top] - 1 : n - 1;
        stk[++top] = i;
    }
    for (int i = 1; i < n; i++)
        lines[i] = {lsh[i], rsh[i] + 1, d[i]};
    cin >> qq;
    for (int i = 1; i <= qq; i++)
        cin >> q[i].l >> q[i].r >> q[i].k, q[i].pos = i;
    sgt.clear();
    sort(q + 1, q + 1 + qq, [](node a, node b)
         { return a.r > b.r; });
    sort(lines + 1, lines + n, [](Line a, Line b)
         { return a.y > b.y; });
    for (int idx = 0, i = 1; i <= qq; i++)
    {
        while (idx < n - 1 && lines[idx + 1].y >= q[i].r)
            idx++, sgt.update(1, 1, n, lines[idx].x, lines[idx].v);
        chkmx(ans[q[i].pos], sgt.query(1, 1, n, 1, q[i].r - q[i].k + 1));
    }
    sgt.clear();
    sort(q + 1, q + 1 + qq, [](node a, node b)
         { return a.k > b.k; });
    sort(lines + 1, lines + n, [](Line a, Line b)
         { return a.y - a.x > b.y - b.x; });
    for (int idx = 0, i = 1; q[i].k > 1; i++)
    {
        while (idx < n - 1 && lines[idx + 1].y - lines[idx + 1].x + 1 >= q[i].k)
            idx++, sgt.update(1, 1, n, lines[idx].y, lines[idx].v);
        chkmx(ans[q[i].pos], sgt.query(1, 1, n, q[i].l + q[i].k - 1, q[i].r));
    }
    sgt.clear();
    sgt.build(1, 1, n);
    sort(q + 1, q + 1 + qq, [](node a, node b)
         { return a.k < b.k; });
    for (int i = 1; q[i].k == 1; i++)
        chkmx(ans[q[i].pos], sgt.query(1, 1, n, q[i].l, q[i].r));
    for (int i = 1; i <= qq; i++)
        cout << ans[i] << endl;
    return 0;
}