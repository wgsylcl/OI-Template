# KMP
```cpp
#include <bits/stdc++.h>
using namespace std;

const int MAXL = 1e6 + 10;
string s, t;
int slen, tlen, nxt[MAXL];
void getnxt()
{
    int j = 0;
    for (int i = 2; i <= tlen; i++)
    {
        while (j && t[j + 1] != t[i])
            j = nxt[j];
        if (t[i] == t[j + 1])
            j++;
        nxt[i] = j;
    }
}
void solve()
{
    int j = 0;
    for (int i = 1; i <= slen; i++)
    {
        while (j && t[j + 1] != s[i])
            j = nxt[j];
        if (s[i] == t[j + 1])
            j++;
        if (j == tlen)
        {
            cout << i - j + 1 << endl;
        }
    }
}
int main()
{
    ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    cin >> s >> t;
    slen = s.size(), tlen = t.size();
    s = " " + s;
    t = " " + t;
    getnxt();
    solve();
    for (int i = 1; i <= tlen; i++)
        cout << nxt[i] << " ";
    return 0;
}
```
