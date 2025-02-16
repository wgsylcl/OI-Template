# 斜率优化DP
```admonish example
[P5785 [SDOI2012] 任务安排](https://www.luogu.com.cn/problem/P5785)
#### 题目描述

机器上有 $n$ 个需要处理的任务，它们构成了一个序列。这些任务被标号为 $1$ 到 $n$，因此序列的排列为 $1 , 2 , 3 \cdots n$。这 $n$ 个任务被分成若干批，每批包含相邻的若干任务。从时刻 $0$ 开始，这些任务被分批加工，第 $i$ 个任务单独完成所需的时间是 $T_i$。在每批任务开始前，机器需要启动时间 $s$，而完成这批任务所需的时间是各个任务需要时间的总和。

**注意，同一批任务将在同一时刻完成**。每个任务的费用是它的完成时刻乘以一个费用系数 $C_i$。

请确定一个分组方案，使得总费用最小。

#### 数据范围

对于 $100\%$ 数据，$1 \le n \le 3 \times 10^5$，$1 \le s \le 2^8$，$ \left| T_i \right| \le 2^8$，$0 \le C_i \le 2^8$。
```

见[这篇题解](https://www.luogu.com.cn/article/ttiqcdc3)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define mid ((l + r) >> 1)
const int MAXN = 3e5 + 10;
int head = 1, tail, q[MAXN], n;
ll s, t[MAXN], c[MAXN], f[MAXN];
int find(int l, int r, ll s)
{
    int ans = tail;
    while (l <= r)
        if (f[q[mid + 1]] - f[q[mid]] > s * (c[q[mid + 1]] - c[q[mid]]))
            ans = mid, r = mid - 1;
        else
            l = mid + 1;
    return q[ans];
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> s;
    for (int i = 1; i <= n; i++)
        cin >> t[i] >> c[i], t[i] += t[i - 1], c[i] += c[i - 1];
    q[++tail] = 0;
    for (int i = 1; i <= n; i++)
    {
        int p = find(head, tail, s + t[i]);
        f[i] = f[p] + s * (c[n] - c[p]) + t[i] * (c[i] - c[p]);
        while (head < tail && (f[q[tail]] - f[q[tail - 1]]) * (c[i] - c[q[tail]]) >= (f[i] - f[q[tail]]) * (c[q[tail]] - c[q[tail - 1]]))
            tail--;
        q[++tail] = i;
    }
    cout << f[n];
    return 0;
}
```