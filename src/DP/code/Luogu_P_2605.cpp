#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define endl '\n'
#define mid ((l + r) >> 1)
#define ls ((x) << 1)
#define rs ((x) << 1 | 1)
inline void chkmn(ll &a, ll b) { a = a > b ? b : a; }
const int MAXN = 2e4 + 10;
const ll inf = 1e18;
vector<int> dr[MAXN];
ll tr[MAXN << 2], d[MAXN], c[MAXN], s[MAXN], L[MAXN], R[MAXN], w[MAXN], ans, lazy[MAXN << 2], f[MAXN];
int n, k;
void addtag(int x, ll v)
{
    tr[x] += v;
    lazy[x] += v;
}
void pushup(int x) { tr[x] = min(tr[ls], tr[rs]); }
void pushdown(int x)
{
    if (lazy[x])
    {
        addtag(ls, lazy[x]);
        addtag(rs, lazy[x]);
        lazy[x] = 0;
    }
}
void build(int x, int l, int r)
{
    lazy[x] = 0;
    if (l >= r)
    {
        tr[x] = f[l];
        return;
    }
    build(ls, l, mid);
    build(rs, mid + 1, r);
    pushup(x);
}
void update(int x, int l, int r, int pl, int pr, ll v)
{
    if (pl > pr)
        return;
    if (pl <= l && r <= pr)
        return addtag(x, v);
    pushdown(x);
    if (pl <= mid)
        update(ls, l, mid, pl, pr, v);
    if (mid < pr)
        update(rs, mid + 1, r, pl, pr, v);
    pushup(x);
}
ll query(int x, int l, int r, int pl, int pr)
{
    if (pl > pr)
        return inf;
    if (pl <= l && r <= pr)
        return tr[x];
    ll res = inf;
    if (pl <= mid)
        chkmn(res, query(ls, l, mid, pl, pr));
    if (mid < pr)
        chkmn(res, query(rs, mid + 1, r, pl, pr));
    return res;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> k;
    for (int i = 2; i <= n; i++)
        cin >> d[i];
    for (int i = 1; i <= n; i++)
        cin >> c[i];
    for (int i = 1; i <= n; i++)
        cin >> s[i];
    for (int i = 1; i <= n; i++)
        cin >> w[i];
    d[++n] = inf;
    ll cur = 0;
    for (int i = 1; i <= n; i++)
    {
        L[i] = lower_bound(d + 1, d + 1 + n, d[i] - s[i]) - d;
        R[i] = upper_bound(d + 1, d + 1 + n, d[i] + s[i]) - d - 1;
        dr[R[i]].push_back(i);
    }
    for (int i = 1; i <= n; i++)
    {
        f[i] = cur + c[i];
        for (int x : dr[i])
            cur += w[x];
    }
    ans = f[n];
    for (int i = 2; i <= k + 1; i++)
    {
        build(1, 1, n);
        for (int j = 1; j <= n; j++)
        {
            f[j] = query(1, 1, n, 1, j - 1) + c[j];
            for (int x : dr[j])
                update(1, 1, n, 1, L[x] - 1, w[x]);
        }
        chkmn(ans, f[n]);
    }
    cout << ans;
    return 0;
}