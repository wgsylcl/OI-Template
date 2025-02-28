# ST表
~~~cpp
#include <bits/stdc++.h>

using namespace std;

#define MAXN 100010

int st[MAXN][20], m, n;
inline int query(int l, int r)
{
    int len = log2(r - l + 1);
    return max(st[l][len], st[r - (1 << len) + 1][len]);
}
inline int read()
{
    char ch = getchar();
    int f = 1, x = 0;
    while (!isdigit(ch))
    {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while (isdigit(ch))
        x = (x << 3) + (x << 1) + (ch - '0'), ch = getchar();
    return f * x;
}
inline void write(int x)
{
    if (x < 0)
    {
        putchar('-');
        write(-x);
        return;
    }
    if (x >= 10)
    {
        write(x / 10);
    }
    putchar(x % 10 + '0');
    return;
}

int main()
{
    n = read(), m = read();
    for (int i = 1; i <= n; i++)
        st[i][0] = read();
    int deep = log2(n);
    for (int d = 1; d <= deep; d++)
        for (int s = 1; s + (1 << d) - 1 <= n; s++)
            st[s][d] = max(st[s][d - 1], st[s + (1 << (d - 1))][d - 1]);
    while (m--)
    {
        int l = read(),r = read();
        write(query(l, r));
        puts("");
    }
    return 0;
}
~~~