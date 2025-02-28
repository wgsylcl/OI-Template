#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
typedef long long ll;
#define mid ((l + r) >> 1)
#define ls ((x) << 1)
#define rs ((x) << 1 | 1)
const int MAXN = 35010, inf = 0x7fffffff;
int dp[60][MAXN], lst[MAXN], pos[MAXN], tr[MAXN << 2], lazy[MAXN << 2];
void addtag(int x, int v)
{
    tr[x] += v;
    lazy[x] += v;
}
void pushup(int x) { tr[x] = max(tr[ls], tr[rs]); }
void pushdown(int x)
{
    if (!lazy[x])
        return;
    addtag(ls, lazy[x]);
    addtag(rs, lazy[x]);
    lazy[x] = 0;
}
void build(int x, int l, int r, int now)
{
    tr[x] = lazy[x] = 0;
    if (l == r)
    {
        tr[x] = dp[now][l - 1];
        return;
    }
    build(ls, l, mid, now);
    build(rs, mid + 1, r, now);
    pushup(x);
}
void update(int x, int l, int r, int pl, int pr, int val)
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
int query(int x, int l, int r, int pl, int pr)
{
    if (pl <= l && r <= pr)
        return tr[x];
    pushdown(x);
    int res = -inf;
    if (pl <= mid)
        res = max(res, query(ls, l, mid, pl, pr));
    if (mid < pr)
        res = max(res, query(rs, mid + 1, r, pl, pr));
    return res;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int n, k, t;
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
    {
        cin >> t;
        lst[i] = pos[t] + 1;
        pos[t] = i;
    }
    for (int i = 1; i <= k; i++)
    {
        build(1, 0, n, i - 1);
        for (int j = 1; j <= n; j++)
        {
            update(1, 0, n, lst[j], j, 1);
            dp[i][j] = query(1, 0, n, 0, j);
        }
    }
    cout << dp[k][n];
    return 0;
}