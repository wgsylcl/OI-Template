#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXL = 30, MAXV = 300, MAXN = 11;
struct node
{
    int fail;
    int son[26];
    int end;
} t[100010];
int node_cnt;
void insert(string p, int idx)
{
    int u = 0;
    for (char &c : p)
    {
        if (!t[u].son[c - 'a'])
            t[u].son[c - 'a'] = ++node_cnt;
        u = t[u].son[c - 'a'];
    }
    t[u].end = 1 << (idx - 1);
}
void getfail()
{
    queue<int> q;
    for (int i = 0; i < 26; i++)
        if (t[0].son[i])
            q.push(t[0].son[i]);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        t[u].end |= t[t[u].fail].end;
        for (int i = 0; i < 26; i++)
        {
            if (t[u].son[i])
            {
                t[t[u].son[i]].fail = t[t[u].fail].son[i];
                q.push(t[u].son[i]);
            }
            else
                t[u].son[i] = t[t[u].fail].son[i];
        }
    }
}
int n, m, top;
ll f[MAXL][MAXV][1 << MAXN], ans;
char stk[MAXL];
bool chk[MAXL][MAXV][1 << MAXN], vis[MAXL][MAXV][1 << MAXN];
inline void getdp()
{
    f[0][0][0] = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= node_cnt; j++)
            for (int k = 0; k < (1 << m); k++)
                if (f[i][j][k])
                    for (int u = 0; u < 26; u++)
                        f[i + 1][t[j].son[u]][k | t[t[j].son[u]].end] += f[i][j][k];
    for (int i = 0; i <= node_cnt; i++)
        ans += f[n][i][(1 << m) - 1];
}
bool dfs(int i, int u, int sta)
{
    if (vis[i][u][sta])
        return chk[i][u][sta];
    vis[i][u][sta] = 1;
    if (i == n)
        return chk[i][u][sta] = (sta == (1 << m) - 1);
    bool res = 0;
    for (int j = 0; j < 26; j++)
        res |= dfs(i + 1, t[u].son[j], sta | t[t[u].son[j]].end);
    return chk[i][u][sta] = res;
}
void print(int i, int u, int sta)
{
    if (!chk[i][u][sta])
        return;
    if (i == n)
    {
        for (int i = 1; i <= n; i++)
            cout << stk[i];
        cout << endl;
        return;
    }
    for (int j = 0; j < 26; j++)
    {
        stk[++top] = j + 'a';
        print(i + 1, t[u].son[j], sta | t[t[u].son[j]].end);
        top--;
    }
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> m;
    string tmp;
    for (int i = 1; i <= m; i++)
        cin >> tmp, insert(tmp, i);
    getfail();
    getdp();
    cout << ans << endl;
    if(ans <= 42)
        dfs(0, 0, 0), print(0, 0, 0);
    return 0;
}