#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
typedef long long ll;
const int MAXM = 1e5 + 10;
ll n, m, k, d, blen;
#define mid ((l + r) >> 1)
#define ls ((x) << 1)
#define rs ((x) << 1 | 1)
struct node
{
    ll l, r, v;
} a[MAXM];
ll b[MAXM << 1], f[MAXM << 1];
bool cmp(node a, node b) { return a.r < b.r; }
ll lazy[MAXM << 3], tr[MAXM << 3];
void pushup(int x) { tr[x] = max(tr[ls], tr[rs]); }
void build(int x, int l, int r)
{
    lazy[x] = 0;
    if (l == r)
    {
        tr[x] = (l ? -1e18 : 0);
        return;
    }
    build(ls, l, mid), build(rs, mid + 1, r);
    pushup(x);
}
void addtag(int x, ll v)
{
    tr[x] += v;
    lazy[x] += v;
}
void pushdown(int x)
{
    if (!lazy[x])
        return;
    addtag(ls, lazy[x]);
    addtag(rs, lazy[x]);
    lazy[x] = 0;
}
ll query(int x, int l, int r, int pl, int pr)
{
    if (pl <= l && r <= pr)
        return tr[x];
    pushdown(x);
    ll ans = -1e18;
    if (pl <= mid)
        ans = max(ans, query(ls, l, mid, pl, pr));
    if (mid < pr)
        ans = max(ans, query(rs, mid + 1, r, pl, pr));
    return ans;
}
void assign(int x, int l, int r, int p, ll val)
{
    if (l == r)
    {
        tr[x] = val;
        return;
    }
    pushdown(x);
    if (p <= mid)
        assign(ls, l, mid, p, val);
    else
        assign(rs, mid + 1, r, p, val);
    pushup(x);
}
void update(int x, int l, int r, int pl, int pr, ll val)
{
    if (pl <= l && r <= pr)
    {
        addtag(x, val);
        return;
    }
    pushdown(x);
    if (pl <= mid)
        update(ls, l, mid, pl, pr, val);
    if (mid < pr)
        update(rs, mid + 1, r, pl, pr, val);
    pushup(x);
}
void solve()
{
    cin >> n >> m >> k >> d;
    blen = 0;
    for (int i = 1; i <= m; i++)
    {
        cin >> a[i].r >> a[i].l >> a[i].v;
        a[i].l = a[i].r - a[i].l;
        b[++blen] = a[i].l, b[++blen] = a[i].r;
    }
    sort(b + 1, b + blen + 1);
    sort(a + 1, a + m + 1, cmp);
    blen = unique(b + 1, b + blen + 1) - b - 1;
    for (int i = 1; i <= m; i++)
    {
        a[i].l = lower_bound(b + 1, b + blen + 1, a[i].l) - b;
        a[i].r = lower_bound(b + 1, b + blen + 1, a[i].r) - b;
    }
    build(1, 0, blen);
    ll ans = 0;
    for (int i = 1, lst = 0, ridx = 1; i <= blen; i++)
    {
        assign(1, 0, blen, i, ans + d * b[i]);
        for (; ridx <= m && a[ridx].r == i; ridx++)
            update(1, 0, blen, 0, a[ridx].l, a[ridx].v);
        for (; lst < i && b[lst] < b[i] - k; lst++)
            ;
        if (lst < i)
            f[i] = query(1, 0, blen, lst, i - 1) - d * b[i];
        else
            f[i] = -1e18;
        ans = max(ans, f[i]);
    }
    cout << ans << endl;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int c, T;
    cin >> c >> T;
    while (T--)
        solve();
    return 0;
}