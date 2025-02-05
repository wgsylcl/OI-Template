# CDQ分治
### 三维偏序
```cpp
#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int N = 2e5 + 5;
const int M = 1e6 + 5;
int n, k, m, tot;
struct BIT
{
    int bit[N];
    inline int lowbit(int x) { return x & -x; }
    inline void add(int x, int d)
    {
        for (; x <= k; x += lowbit(x))
            bit[x] += d;
    }
    inline int query(int x)
    {
        int res = 0;
        for (; x; x -= lowbit(x))
            res += bit[x];
        return res;
    }
} bit;
struct Node
{
    int a, b, c, cnt, ans;
} ss[N], s[N];
int ans[N];
bool cmpA(Node a, Node b)
{
    if (a.a == b.a)
    {
        return a.b == b.b ? a.c < b.c : a.b < b.b;
    }
    return a.a < b.a;
}
bool cmpB(Node a, Node b)
{
    return a.b == b.b ? a.c < b.c : a.b < b.b;
}
void cdq(int l, int r)
{
    if (l >= r)
        return;
    int mid = l + r >> 1;
    cdq(l, mid);
    cdq(mid + 1, r);
    int i, j = l;
    sort(s + l, s + mid + 1, cmpB);
    sort(s + mid + 1, s + r + 1, cmpB);
    for (i = mid + 1; i <= r; i++)
    {
        while (s[i].b >= s[j].b && j <= mid)
        {
            bit.add(s[j].c, s[j].cnt);
            j++;
        }
        s[i].ans += bit.query(s[i].c);
    }
    for (i = l; i < j; i++)
        bit.add(s[i].c, -s[i].cnt);
}
int main()
{
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
    {
        scanf("%d%d%d", &ss[i].a, &ss[i].b, &ss[i].c);
    }
    sort(ss + 1, ss + n + 1, cmpA);
    for (int i = 1; i <= n; i++)
    {
        tot++;
        if (ss[i].a != ss[i + 1].a || ss[i].b != ss[i + 1].b || ss[i].c != ss[i + 1].c)
        {
            s[++m] = ss[i];
            s[m].cnt = tot;
            tot = 0;
        }
    }
    cdq(1, m);
    for (int i = 1; i <= n; i++)
        ans[s[i].cnt + s[i].ans - 1] += s[i].cnt;
    for (int i = 0; i < n; i++)
        printf("%d\n", ans[i]);
    return 0;
}
```

### 优化1D/1D类dp (P4093)
```cpp
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1e5 + 10, inf = 0x7fffffff, MAXV = 1e5 + 10;
struct node
{
    int mx, mn, val, pos;
} a[MAXN];
namespace bit
{
    int tr[MAXV];
    inline int lowbit(int x) { return x & (-x); }
    inline void update(int x, int val)
    {
        while (x <= 1e5)
        {
            tr[x] = max(tr[x], val);
            x += lowbit(x);
        }
    }
    inline int query(int x)
    {
        int res = 0;
        while (x)
        {
            res = max(res, tr[x]);
            x -= lowbit(x);
        }
        return res;
    }
    inline void clear(int x)
    {
        while (x <= 1e5)
        {
            tr[x] = 0;
            x += lowbit(x);
        }
    }
}
int dp[MAXN], n, m, ans;
void cdq(int l, int r)
{
    if (l >= r)
    {
        dp[l] = max(dp[l], 1);
        return;
    }
    int mid = (l + r) >> 1;
    sort(a + l, a + r + 1, [](node p, node q)
         { return p.pos < q.pos; });
    cdq(l, mid);
    sort(a + l, a + mid + 1, [](node p, node q)
         { return p.mx < q.mx; });
    sort(a + mid + 1, a + r + 1, [](node p, node q)
         { return p.val < q.val; });
    int i = l, j;
    // a[i].mx <= a[j].val && a[i].val <= a[j].mn
    for (j = mid + 1; j <= r; j++)
    {
        while (a[i].mx <= a[j].val && i <= mid)
        {
            bit::update(a[i].val, dp[a[i].pos]);
            i++;
        }
        dp[a[j].pos] = max(dp[a[j].pos], bit::query(a[j].mn) + 1);
    }
    while (i >= l)
        bit::clear(a[i--].val);
    cdq(mid + 1, r);
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        cin >> a[i].val, a[i].mx = a[i].mn = a[i].val, a[i].pos = i;
    while (m--)
    {
        int x, y;
        cin >> x >> y;
        a[x].mn = min(a[x].mn, y);
        a[x].mx = max(a[x].mx, y);
    }
    cdq(1, n);
    for (int i = 1; i <= n; i++)
        ans = max(ans, dp[i]);
    cout << ans;
    return 0;
}
```