#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 1e6 + 5;
const int mod = 1e8 + 7;
int n, m, f[N], a[N];
int qpow(int n, int k)
{
    int ans = 1;
    while (k)
    {
        if (k & 1)
            ans = ans * n % mod;
        n = n * n % mod;
        k >>= 1;
    }
    return ans;
}
int fac[N];
signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, m;
    cin >> n >> m;
    int p = qpow(2, n) - 1;
    a[0] = 1, fac[0] = 1;
    for (int i = 1; i <= m; i++)
    {
        a[i] = a[i - 1] * (p - i + 1 + mod) % mod;
        fac[i] = fac[i - 1] * i % mod;
    }
    f[0] = 1;
    for (int i = 2; i <= m; i++)
    {
        f[i] = a[i - 1];
        f[i] = (f[i] - f[i - 1] + mod) % mod;
        f[i] = ((f[i] - f[i - 2] * (i - 1) % mod * (p - i + 2)) % mod + mod) % mod;
    }
    // cout << f[2] << endl << f[3] << endl;
    cout << f[m] * qpow(fac[m], mod - 2) % mod;
    return 0;
}