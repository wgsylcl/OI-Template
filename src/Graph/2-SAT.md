# 2-SAT
~~~cpp
#include <iostream>
#include <cstdio>
#include <stack>
#include <queue>
#include <vector>
using namespace std;
namespace IO
{
#define siz 1 << 20
    char buf[siz], *pa = buf, *pb = buf;
#define gc() (pa == pb && (pb = (pa = buf) + fread(buf, 1, siz, stdin), pa == pb)) ? EOF : *(pa++)
    inline int read()
    {
        int x = 0, f = 1;
        char ch;
        for (ch = gc(); !isdigit(ch); ch = gc())
            if (ch == '-')
                f = -1;
        for (; isdigit(ch); ch = gc())
        {
            x = (x << 3) + (x << 1) + (ch - '0');
        }
        return f * x;
    }
    inline void write(int x)
    {
        if (x < 0)
        {
            putchar('-');
            x = -x;
        }
        if (x > 9)
            write(x / 10);
        putchar(x % 10 + '0');
        return;
    }
#undef siz
#undef gc
}
using IO::read;
using IO::write;
#define truep(x) (x)
#define falsep(x) ((x) + n)
#define MAXN 1000010 << 1
#define maxm 1000010 << 1
vector<int> e[MAXN];
stack<int> s;
queue<int> q;
int n, m, color[MAXN], idx, cnt, ind[MAXN], low[MAXN], dfn[MAXN];
bool book[MAXN];
void dfs(int u)
{
    dfn[u] = low[u] = ++idx;
    s.push(u), book[u] = true;
    for (int i = 0; i < e[u].size(); i++)
    {
        int v = e[u][i];
        if (!dfn[v])
        {
            dfs(v);
            low[u] = min(low[u], low[v]);
        }
        else if (book[v])
            low[u] = min(low[u], dfn[v]);
    }
    if (low[u] == dfn[u])
    {
        cnt++;
        int t;
        do
        {
            t = s.top();
            s.pop();
            color[t] = cnt;
            book[t] = false;
        } while (t != u);
    }
}
int main()
{
    n = read(), m = read();
    for (int i = 1; i <= m; i++)
    {
        int u = read(), a = read(), v = read(), b = read();
        e[truep(u) * a + falsep(u) * (a ^ 1)].push_back(truep(v) * (b ^ 1) + falsep(v) * b);
        e[truep(v) * b + falsep(v) * (b ^ 1)].push_back(truep(u) * (a ^ 1) + falsep(u) * a);
    }
    for (int i = 1; i <= (n << 1); i++)
        if (!dfn[i])
            dfs(i);
    for (int i = 1; i <= n; i++)
    {
        if (color[truep(i)] == color[falsep(i)])
        {
            puts("IMPOSSIBLE");
            return 0;
        }
    }
    puts("POSSIBLE");
    for (int i = 1; i <= n; i++)
    {
        write(color[truep(i)] > color[falsep(i)]), putchar(' ');
    }
    return 0;
}
~~~