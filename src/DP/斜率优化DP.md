# 斜率优化DP
```admonish question title = "[「HNOI2008」玩具装箱](https://www.luogu.com.cn/problem/P3195)"
有 $n$ 个玩具，第 $i$ 个玩具价值为 $c_i$。要求将这 $n$ 个玩具排成一排，分成若干段。对于一段 $[l,r]$，它的代价为 $(r-l+\sum_{i=l}^r c_i-L)^2$。其中 $L$ 是一个常量，求分段的最小代价。
    
$1\le n\le 5\times 10^4, 1\le L, c_i\le 10^7$。
```

### 朴素的 DP 做法

令 $f_i$ 表示前 $i$ 个物品，分若干段的最小代价。

状态转移方程：$f_i=\min_{j<i}\{f_j+(i-(j+1)+pre_i-pre_j-L)^2\}=\min_{j<i}\{f_j+(pre_i-pre_j+i-j-1-L)^2\}$。

其中 $pre_i$ 表示前 $i$ 个数的和，即 $\sum_{j=1}^i c_j$。

该做法的时间复杂度为 $O(n^2)$，无法解决本题。

### 优化

考虑简化上面的状态转移方程式：令 $s_i=pre_i+i,L'=L+1$，则 $f_i=\min_{j<i}\{f_j+(s_i-s_j-L')^2\}$。

将与 $j$ 无关的移到外面，我们得到

$$
f_i - (s_i-L')^2=\min_{j<i}\{f_j+s_j^2 + 2s_j(L'-s_i) \} 
$$

考虑一次函数的斜截式 $y=kx+b$，将其移项得到 $b=y-kx$。我们将与 $j$ 有关的信息表示为 $y$ 的形式，把同时与 $i,j$ 有关的信息表示为 $kx$，把要最小化的信息（与 $i$ 有关的信息）表示为 $b$，也就是截距。具体地，设

$$
\begin{aligned}
x_j&=s_j\\
y_j&=f_j+s_j^2\\
k_i&=-2(L'-s_i)\\
b_i&=f_i-(s_i-L')^2\\
\end{aligned}
$$

则转移方程就写作 $b_i = \min_{j<i}\{ y_j-k_ix_j \}$。我们把 $(x_j,y_j)$ 看作二维平面上的点，则 $k_i$ 表示直线斜率，$b_i$ 表示一条过 $(x_j,y_j)$ 的斜率为 $k_i$ 的直线的截距。问题转化为了，选择合适的 $j$（$1\le j<i$），最小化直线的截距。

![slope\_optimization](./slope1.png)

如图，我们将这个斜率为 $k_i$ 的直线从下往上平移，直到有一个点 $(x_p,y_p)$ 在这条直线上，则有 $b_i=y_p-k_ix_p$，这时 $b_i$ 取到最小值。算完 $f_i$，我们就把 $(x_i,y_i)$ 这个点加入点集中，以做为新的 DP 决策。那么，我们该如何维护点集？

容易发现，可能让 $b_i$ 取到最小值的点一定在下凸壳上。因此在寻找 $p$ 的时候我们不需要枚举所有 $i-1$ 个点，只需要考虑凸包上的点。而在本题中 $k_i$ 随 $i$ 的增加而递增，因此我们可以单调队列维护凸包。

具体地，设 $K(a,b)$ 表示过 $(x_a,y_a)$ 和 $(x_b,y_b)$ 的直线的斜率。考虑队列 $q_l,q_{l+1},\ldots,q_r$，维护的是下凸壳上的点。也就是说，对于 $l<i<r$，始终有 $K(q_{i-1},q_i) < K(q_i,q_{i+1})$ 成立。

我们维护一个指针 $e$ 来计算 $b_i$ 最小值。我们需要找到一个 $K(q_{e-1},q_e)\le k_i< K(q_e,q_{e+1})$ 的 $e$（特别地，当 $e=l$ 或者 $e=r$ 时要特别判断），这时就有 $p=q_e$，即 $q_e$ 是 $i$ 的最优决策点。由于 $k_i$ 是单调递增的，因此 $e$ 的移动次数是均摊 $O(1)$ 的。

在插入一个点 $(x_i,y_i)$ 时，我们要判断是否 $K(q_{r-1},q_r)<K(q_r,i)$，如果不等式不成立就将 $q_r$ 弹出，直到等式满足。然后将 $i$ 插入到 $q$ 队尾。

这样我们就将 DP 的复杂度优化到了 $O(n)$。

概括一下上述斜率优化模板题的算法：

1.  将初始状态入队。
2.  每次使用一条和 $i$ 相关的直线 $f(i)$ 去切维护的凸包，找到最优决策，更新 $dp_i$。
3.  加入状态 $dp_i$。如果一个状态（即凸包上的一个点）在 $dp_i$ 加入后不再是凸包上的点，需要在 $dp_i$ 加入前将其剔除。

接下来介绍更多的例子

### 点积最大化

```admonish question title="[P3648 [APIO2014] 序列分割](https://www.luogu.com.cn/problem/P3648)"

你正在玩一个关于长度为 $n$ 的非负整数序列的游戏。这个游戏中你需要把序列分成 $k + 1$ 个非空的块。为了得到 $k + 1$ 块，你需要重复下面的操作 $k$ 次：

选择一个有超过一个元素的块（初始时你只有一块，即整个序列）

选择两个相邻元素把这个块从中间分开，得到两个非空的块。

每次操作后你将获得那两个新产生的块的元素和的乘积的分数。你想要最大化最后的总得分。

对全部的数据，满足 $2 \leq n \leq 100000, 1 \leq k \leq \min\{n - 1, 200\}$。

```

根据乘法分配律，我们容易知道最终得分与分割顺序无关

那么我们设$f_{i,j}$为前$i$个元素分$j$次的最大的分，容易知道$j$可以滚动数组优化，记$s_i$为元素前缀和，$g$是上一层状态，那么:

$$f_i = \min_{j = 0}^{i - 1}\{g_j + s_j*(s_i - s_j)\} = \min_{j = 0}^{i - 1}\{s_j*s_i + g_j - s_j^2\}$$

这里$s_j*s_i + g_j - s_j^2$可看作是向量$(s_i, 1)$与$(s_j,g_j - s_j^2)$的点积，那么这就是一个点积最大化问题，根据向量点积几何意义，只要让决策点在$(s_i, 1)$距离最大就行，如下图

![](./slope2.png)

因此维护上凸包即可

```cpp
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
typedef long long ll;
const int MAXN = 1e5 + 10;
ll f[2][MAXN], pre[210][MAXN];
int head, tail, n, k, a[MAXN];
struct vec
{
    int id, x;
    ll y;
    ll operator()(int v) { return (ll)x * v + y; }
} q[MAXN];
ll cross(vec a, vec b, vec c)
{
    a.x -= b.x;
    a.y -= b.y;
    b.x -= c.x;
    b.y -= c.y;
    return b.x * a.y - a.x * b.y;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
        cin >> a[i], a[i] += a[i - 1];
    for (int t = 1; t <= k; t++)
    {
        head = tail = 0;
        q[0] = {0, 0, 0};
        for (int i = 1; i <= n; i++)
        {
            while (head < tail && q[head](a[i]) <= q[head + 1](a[i]))
                head++;
            f[t & 1][i] = q[head](a[i]);
            pre[t][i] = q[head].id;
            vec x = {i, a[i], f[t & 1 ^ 1][i] - (ll)a[i] * a[i]};
            while (head < tail && cross(q[tail - 1], q[tail], x) <= 0)
                tail--;
            q[++tail] = x;
        }
    }
    cout << f[k & 1][n] << endl;
    int now = n;
    for (int i = k; i >= 1; i--)
        cout << pre[i][now] << " ", now = pre[i][now];
    return 0;
}
```

接下来我们介绍斜率优化的进阶应用，将斜率优化与二分/分治/数据结构等结合，来维护性质不那么好（缺少一些单调性性质）的 DP 方程。

### 二分优化

```admonish question title="[P5785 [SDOI2012] 任务安排](https://www.luogu.com.cn/problem/P5785)"

机器上有 $n$ 个需要处理的任务，它们构成了一个序列。这些任务被标号为 $1$ 到 $n$，因此序列的排列为 $1 , 2 , 3 \cdots n$。这 $n$ 个任务被分成若干批，每批包含相邻的若干任务。从时刻 $0$ 开始，这些任务被分批加工，第 $i$ 个任务单独完成所需的时间是 $T_i$。在每批任务开始前，机器需要启动时间 $s$，而完成这批任务所需的时间是各个任务需要时间的总和。

**注意，同一批任务将在同一时刻完成**。每个任务的费用是它的完成时刻乘以一个费用系数 $C_i$。

请确定一个分组方案，使得总费用最小。

#### 数据范围

对于 $100\%$ 数据，$1 \le n \le 3 \times 10^5$，$1 \le s \le 2^8$，$ \left| T_i \right| \le 2^8$，$0 \le C_i \le 2^8$。
```

我们发现，查询直线的斜率不再单调，但是加入的决策点横坐标依旧单调

因此我们在凸包上二分找决策点即可

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