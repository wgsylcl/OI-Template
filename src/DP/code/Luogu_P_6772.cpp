#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 260;
const ll inf = 1e9;
struct Matrix
{
    ll lx, ly;
    ll val[MAXN][MAXN];
    Matrix() : lx(0), ly(0)
    {
        for (int i = 0; i < MAXN; i++)
            for (int j = 0; j < MAXN; j++)
                val[i][j] = -inf;
    }
    inline ll *operator[](const int i) { return val[i]; }
} g[30], dp;
inline Matrix operator*(Matrix a, Matrix b)
{
    Matrix c;
    c.lx = a.lx, c.ly = b.ly;
    for (int i = 1; i <= a.lx; i++)
        for (int k = 1; k <= a.ly; k++)
            for (int j = 1; j <= b.ly; j++)
                c[i][j] = max(c[i][j], a[i][k] + b[k][j]);
    return c;
}
int n, m, k, t;
ll c[MAXN];
inline int getid(int u, int i) { return (u - 1) * 5 + i; }
struct Festival
{
    int t, x, y;
} fst[MAXN];
inline void pow(Matrix &a, int b)
{
    if (!b)
        return;
    for (int i = 0; i <= log2(b); i++)
        if (b & (1 << i))
            a = g[i] * a;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> m >> t >> k;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j < 5; j++)
            g[0][getid(i, j + 1)][getid(i, j)] = 0;
    for (int i = 1; i <= n; i++)
        cin >> c[i];
    g[0].lx = g[0].ly = n * 5;
    dp.lx = n * 5, dp.ly = 1;
    while (m--)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g[0][getid(v, 1)][getid(u, w)] = max(g[0][getid(v, 1)][getid(u, w)], c[v]);
    }
    for (int i = 1; i <= log2(t); i++)
        g[i] = g[i - 1] * g[i - 1];
    dp[1][1] = 0;
    for (int i = 1; i <= k; i++)
        cin >> fst[i].t >> fst[i].x >> fst[i].y;
    sort(fst + 1, fst + 1 + k, [](Festival x, Festival y)
         { return x.t < y.t; });
    int now = 0;
    for (int i = 1; i <= k; i++)
        pow(dp, fst[i].t - now), dp[getid(fst[i].x, 1)][1] += fst[i].y, now = fst[i].t;
    pow(dp, t - now);
    cout << (dp[1][1] < 0 ? -1 : dp[1][1] + c[1]);
    return 0;
}