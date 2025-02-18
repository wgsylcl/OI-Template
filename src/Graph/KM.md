# KM
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
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
#define MAXN 1000
#define left(x) (x)
#define right(x) (x + n)
vector<int> e[MAXN + 10];
bool book[MAXN + 10];
int m, n, ed, match[MAXN + 10], ans;
bool dfs(int u)
{
    for (int i = 0; i < e[u].size(); i++)
    {
        int v = e[u][i];
        if (!book[v])
        {
            book[v] = true;
            if (!match[v] || dfs(match[v]))
            {
                match[v] = u;
                return true;
            }
        }
    }
    return false;
}
int main()
{
    n = read(), m = read(), ed = read();
    for (int i = 0; i < ed; i++)
    {
        int u = read(), v = read();
        e[u].push_back(right(v));
    }
    for (int i = 1; i <= n; i++)
    {
        memset(book, 0, sizeof(book));
        if (dfs(i))
            ans++;
    }
    cout << ans;
    return 0;
}
```