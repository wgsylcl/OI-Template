# manacher
~~~cpp
#include <bits/stdc++.h>
using namespace std;

int p[22000010], ans, c, r;
string text;
string change(string s)
{
    string ret = "@";
    for (char ch : s)
        ret += '#', ret += ch;
    return ret + "#&";
}
void manacher(string s)
{
    for (int i = 1; i < s.size() - 1; i++)
    {
        if (i <= r)
            p[i] = min(p[(c << 1) - i], p[c] + c - i);
        while (s[i + p[i]] == s[i - p[i]])
            p[i]++;
        if (i + p[i] > r)
            r = i + p[i] - 1, c = i;
    }
}
int main()
{
    ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    getline(cin, text);
    text = change(text);
    manacher(text);
    for (int i = 0; i < text.size(); i++)
        ans = max(ans, p[i]);
    cout << ans - 1;
    return 0;
}
~~~