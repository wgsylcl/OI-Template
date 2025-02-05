# AC自动机
```cpp
#include <bits/stdc++.h>
using namespace std;

struct node
{
    int fail;
    int son[26];
    int end;
} t[100010];
struct Mdata
{
    string s;
    int cnt, pos;
    bool operator<(Mdata &a)
    {
        if (cnt != a.cnt)
            return cnt > a.cnt;
        return pos < a.pos;
    }
} ds[200];
int n, node_cnt;
void insert(string p, int idx)
{
    int u = 0;
    for (char &c : p)
    {
        if (!t[u].son[c - 'a'])
            t[u].son[c - 'a'] = ++node_cnt;
        u = t[u].son[c - 'a'];
    }
    t[u].end = idx;
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
void query(string text)
{
    int u = 0;
    for (char &c : text)
    {
        u = t[u].son[c - 'a'];
        int temp = u;
        while (temp)
        {
            if (t[temp].end)
                ds[t[temp].end].cnt++;
            temp = t[temp].fail;
        }
    }
}
bool solve()
{
    memset(t, 0, sizeof(t));
    node_cnt = 0;
    cin >> n;
    if (!n)
        return false;
    for (int i = 1; i <= n; i++)
    {
        cin >> ds[i].s;
        ds[i].pos = i;
        ds[i].cnt = 0;
        insert(ds[i].s, i);
    }
    getfail();
    string text;
    cin >> text;
    query(text);
    sort(ds + 1, ds + n + 1);
    int ans = ds[1].cnt;
    cout << ans << endl;
    for (int i = 1; i <= n && ds[i].cnt == ans; i++)
        cout << ds[i].s << endl;
    return true;
}

int main()
{
    ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    while (solve());
    return 0;
}
```