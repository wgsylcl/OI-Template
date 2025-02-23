# 矩阵加速DP
矩阵快速幂优化动态规划（DP）是一种利用矩阵快速幂技术来加速动态规划计算的方法。以下是对这种方法的详细介绍：

### 矩阵加速DP的原理
- **状态转移矩阵**：在动态规划中，状态转移方程描述了当前状态如何由前一个状态推导而来。通过将状态转移方程表示为矩阵形式，可以将状态转移过程转化为矩阵乘法操作。
- **矩阵幂次**：如果状态转移矩阵是固定的，那么可以通过计算矩阵的幂次来快速推导出多个状态之间的关系。矩阵快速幂算法可以高效地计算矩阵的幂次，从而加速动态规划的计算过程。

### 矩阵加速DP的步骤
1. **定义状态**：确定动态规划的状态变量和状态转移方程。
2. **构建状态转移矩阵**：将状态转移方程表示为矩阵形式，构建状态转移矩阵。
3. **计算矩阵幂次**：利用矩阵快速幂算法计算状态转移矩阵的幂次。
4. **求解最终结果**：通过矩阵乘法操作，将初始状态与状态转移矩阵的幂次相乘，得到最终结果。

### 优点
- **时间复杂度降低**：矩阵快速幂算法的递推次数为$O(\log n)$，相比传统的动态规划方法，可以显著降低计算时间。
- **适用范围广**：适用于状态转移矩阵固定且状态转移方程可以表示为矩阵形式的动态规划问题。
- **代码实现简洁**：矩阵加速DP的代码实现相对简洁，易于理解和维护。

### 局限性
- **状态转移矩阵的规模**：如果状态转移矩阵的规模较大，矩阵乘法的计算量也会相应增加，可能会影响算法的效率。
- **状态转移矩阵的固定性**：矩阵加速DP要求状态转移矩阵是固定的，如果状态转移矩阵随时间变化，则无法直接应用该方法。

### 示例

```admonish question title = "[P4910 帕秋莉的手环](https://www.luogu.com.cn/problem/P4910)"
经过数年魔法的沉淀，帕秋莉将她那浩瀚无边的魔法的一部分浓缩到了一些特质的珠子中。

由于帕秋莉爱好和平，她只把象征生命和觉醒的木属性魔法和果实和丰收的金属性魔法放入了珠子中。

她认为光要这些珠子没有什么用处，于是她想将这些珠子串成魔法手环，这样就好看多了。于是，她拿出来用来串这些珠子的线 - 雾雨灵径。

她将这些珠子串到一起之后发现了一些性质：一段雾雨灵径的颜色是由两边的珠子的属性决定的，当一段雾雨灵径连接的两个珠子中只要有一个是金属性的，那么这段雾雨灵径的颜色就为金色

帕秋莉想要一个全都是金色的手环，而且她还想知道一共有多少种方案。由于她还要研究新的魔法，她就把这件事交给了你。由于她的魔法浩瀚无边，她有无穷的珠子

她并不想看着好几十位的数字，于是你需要对 $1000000007$ 进行取模

本题多测，数据组数为$T$

对于全部的数据，有 $1\le T \le 10,  1\le n \le 10^{18}$
```

我们记$f_{i,0}$为第$i$位是木属性的方案数，$f_{i,1}$为第$i$位是金属性的方案数，那么有转移方程
$$\begin{aligned}
    f_{i,0} &= f_{i-1,1} \\
    f_{i,1} &= f_{i-1,0} + f_{i-1,1}
\end{aligned}$$
线性递推是$O(n)$的，无法接受

观察转移方程，发现很像矩阵乘法，考虑令向量$\boldsymbol{p_i} = \begin{bmatrix}
    f_{i,0} \\
    f_{i,1}
\end{bmatrix}$，容易解出转移矩阵$\boldsymbol{A} = \begin{bmatrix}
    0 & 1 \\
    1 & 1
\end{bmatrix}$

问题来了，初始向量$\boldsymbol{p_1}$是什么，答案又如何统计？

考虑所有可能的首尾组合：
1. 金 - 金
2. 金 - 木
3. 木 - 金

容易发现，情况2和情况3是相同的（翻转一下就行）

因此令$\boldsymbol{p_1} = \begin{bmatrix}
    0 \\
    1
\end{bmatrix}$，则答案为$2f_{n,0}+f_{n,1}$

```cpp
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
typedef long long ll;
const ll mod = 1e9 + 7;
inline void add(ll &a, ll b)
{
    a = (a + b) % mod;
}
struct Matrix
{
    int lx, ly;
    ll val[3][3];
    inline void init()
    {
        for (int i = 1; i <= min(lx, ly); i++)
            val[i][i] = 1;
    }
    inline ll *operator[](const int i)
    {
        return val[i];
    }
} trans, vec;
Matrix operator*(Matrix a, Matrix b)
{
    Matrix c = {a.lx, b.ly, {}};
    for (int i = 1; i <= a.lx; i++)
        for (int k = 1; k <= a.ly; k++)
            for (int j = 1; j <= b.ly; j++)
                add(c[i][j], a[i][k] * b[k][j] % mod);
    return c;
}
Matrix qpow(Matrix a, ll b)
{
    Matrix c = {a.lx, a.ly, {}};
    for (c.init(); b; b >>= 1ll, a = a * a)
        if (b & 1ll)
            c = c * a;
    return c;
}
void init()
{
    vec.lx = 2, vec.ly = 1;
    vec[2][1] = 1;
    trans.lx = trans.ly = 2;
    trans[1][2] = trans[2][1] = trans[2][2] = 1;
}
void solve()
{
    ll n;
    cin >> n;
    Matrix tvec = qpow(trans, n - 1) * vec;
    cout << (tvec[1][1] * 2 + tvec[2][1]) % mod << endl;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    init();
    int T;
    cin >> T;
    while (T--)
        solve();
    return 0;
}
```

```admonish question title = "[P1357 花园](https://www.luogu.com.cn/problem/P1357)"

小 L 有一座环形花园，沿花园的顺时针方向，他把各个花圃编号为 $1 \sim n$。花园 $1$ 和 $n$ 是相邻的。

他的环形花园每天都会换一个新花样，但他的花园都不外乎一个规则：任意相邻 $m$ 个花圃中都只有不超过 $k$ 个 C 形的花圃，其余花圃均为 P 形的花圃。

例如，若 $n=10$ , $m=5$ , $k=3$ ，则

- `CCPCPPPPCC` 是一种不符合规则的花圃。
- `CCPPPPCPCP` 是一种符合规则的花圃。

请帮小 L 求出符合规则的花园种数对 $10^9+7$ 取模的结果。

- 对于 $100\%$ 的数据，保证 $2 \leq n \le 10^{15}$，$2 \leq m \leq \min(n, 5)$，$1 \leq k \lt m$。
```

这是刚刚那题的加强版，考虑到$m$很小，考虑状压

先考虑普通的DP

从第二个样例来看，设状态用二进制表示，有如下状态转移情况：
 - `00` 可以转移到 `00` 和 `01`；
 - `01` 可以转移到 `10`；
 - `10` 可以转移到 `00` 和 `01`。

可以知道前状态的最右几位要和后状态相同，不过最前面的一位被挤出去了，有没有摆C花没有关系，`0` 和 `1` 皆可。因此设前状态为 `i`，后状态为 `j`，可以得到状态转移关系：
$i = \begin{cases}
j >> 1 \\
(j >> 1) | (1 << (m - 1))
\end{cases}$

另设 $f[i][j]$ 为到第 $i$ 盆花，前面 $m$ 盆的状态为 $j$，可以由上面的关系得到DP方程：
$f[i][j] = f[i][j] + \begin{cases}f[i - 1][j >> 1]\\f[i - 1][(j >> 1) | (1 << (m - 1))]\end{cases}$
需要判一下第二个转移的前状态是否合法。

然后要处理环形的问题，可以简单的断环成链，然后对于给定初状态 `statu`，统计DP后的 $f[n][statu]$ 计入答案。这是因为这样对于一个长度为 $n$ 的环，$f[0][statu]$ 和 $f[n][statu]$ 是等价的，所以 `statu` 相同的时候就是一个合法的环。

然后想着 $f[i][j]$ 只和 $f[i - 1][j]$ 有关，所以第一维 $i$ 可以滚动优化掉，然后按照这个 $n$ 的范围肯定是得上矩阵乘法的，试着构造转移矩阵。还是以上面的 $m$ 为例，状态转移还是一样的，设 $f'[j]$ 为状态为 $j$ 时的方案数，由 $f[j]$ 推出 $f'[j]$，就是下一轮的 $f[j]$；得到方程：
$$\begin{cases}
f'[00] = f[00] + f[10]\\
f'[01] = f[00] + f[10]\\
f'[10] = f[01]\\
f'[11] = 0
\end{cases}$$
构造出转移矩阵：
$$\begin{bmatrix}
1 & 0 & 1 & 0\\
1 & 0 & 1 & 0\\
0 & 1 & 0 & 0\\
0 & 0 & 0 & 0
\end{bmatrix}$$
对于任意的 $m$，也可以通过前后两个状态的关系，令转移矩阵中的元素为 `1` 或 `0`，构造出转移矩阵。

另外还有一点，对于之前每一个状态做一遍DP，现在我们有了矩阵，就没有必要对于每个初状态各做一次了，可以把初始矩阵的第一维利用起来，即令初始矩阵为单位矩阵可以让每一种初状态并行处理，每一行中就是我们要的初状态只有一种的一轮DP。然而根据矩阵乘法的性质，这个矩阵乘上任意矩阵都等于所乘的矩阵，也就是说是一个单位矩阵，所以乘不乘没差，那么我们所求的答案就是转移矩阵自乘 $n$ 次之后，其主对角线元素之和。

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXM = 7, MAXV = 1 << MAXM;
const ll p = 1e9 + 7;
inline ll mod(const ll x) { return x % p; }
ll n, m, k, sz, ans;
struct matrix
{
    ll val[MAXV][MAXV];
    inline ll *operator[](const int i) { return val[i]; }
    matrix() : val({}) {}
    inline void init()
    {
        for (int i = 0; i < sz; i++)
            val[i][i] = 1;
    }
    friend inline matrix operator*(matrix a, matrix b)
    {
        matrix res;
        for (int i = 0; i < sz; i++)
            for (int j = 0; j < sz; j++)
                for (int kk = 0; kk < sz; kk++)
                    res[i][j] = mod(res[i][j] + mod(a[i][kk] * b[kk][j]));
        return res;
    }
    inline matrix pow(ll b)
    {
        matrix res, a = *this;
        res.init();
        while (b)
        {
            if (b & 1ll)
                res = res * a;
            a = a * a;
            b >>= 1ll;
        }
        return res;
    }
} mat, amat;
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> m >> k;
    sz = 1 << m;
    for (int i = 0, j; i < sz; i++)
    {
        if (__builtin_popcount(i) > k)
            continue;
        j = i >> 1;
        mat[j][i] = 1;
        j = (i >> 1) | (1 << (m - 1));
        if (__builtin_popcount(j) <= k)
            mat[j][i] = 1;
    }
    amat = mat.pow(n);
    for (int i = 0; i < sz; i++)
        ans = mod(ans + amat[i][i]);
    cout << ans;
    return 0;
}
```

### 与图论结合

#### 定长路径统计

```admonish question
给一个 $n$ 阶有向图，每条边的边权均为 $1$，然后给一个整数 $k$，你的任务是对于所有点对 $(u,v)$ 求出从 $u$ 到 $v$ 长度为 $k$ 的路径的数量（不一定是简单路径，即路径上的点或者边可能走多次）。
```

我们将这个图用邻接矩阵 $G$（对于图中的边 $(u\to v)$，令 $G[u,v]=1$，其余为 $0$ 的矩阵；如果有重边，则设 $G[u,v]$ 为重边的数量）表示这个有向图。下述算法同样适用于图有自环的情况。

显然，该邻接矩阵对应 $k=1$ 时的答案。

假设我们知道长度为 $k$ 的路径条数构成的矩阵，记为矩阵 $C_k$，我们想求 $C_{k+1}$。显然有 DP 转移方程

$$
C_{k+1}[i,j] = \sum_{p = 1}^{n} C_k[i,p] \cdot G[p,j]
$$

我们可以把它看作矩阵乘法的运算，于是上述转移可以描述为

$$
C_{k+1} = C_k \cdot G
$$

那么把这个递推式展开可以得到

$$
C_k = \underbrace{G \cdot G \cdots G}_{k \text{ 次}} = G^k
$$

要计算这个矩阵幂，我们可以使用快速幂（二进制取幂）的思想，在 $O(n^3 \log k)$ 的复杂度内计算结果。

#### 定长最短路

```admonish question
给你一个 $n$ 阶加权有向图和一个整数 $k$。对于每个点对 $(u,v)$ 找到从 $u$ 到 $v$ 的恰好包含 $k$ 条边的最短路的长度。（不一定是简单路径，即路径上的点或者边可能走多次）
```

我们仍构造这个图的邻接矩阵 $G$，$G[i,j]$ 表示从 $i$ 到 $j$ 的边权。如果 $i,j$ 两点之间没有边，那么 $G[i,j]=\infty$。（有重边的情况取边权的最小值）

显然上述矩阵对应 $k=1$ 时问题的答案。我们仍假设我们知道 $k$ 的答案，记为矩阵 $L_k$。现在我们想求 $k+1$ 的答案。显然有转移方程

$$
L_{k+1}[i,j] = \min_{1\le p \le n} \left\{L_k[i,p] + G[p,j]\right\}
$$

事实上我们可以类比矩阵乘法，你发现上述转移只是把矩阵乘法的乘积求和变成相加取最小值，于是我们定义这个运算为 $\odot$，即

$$
A \odot B = C~~\Longleftrightarrow~~C[i,j]=\min_{1\le p \le n}\left\{A[i,p] + B[p,j]\right\}
$$

于是得到

$$
L_{k+1} = L_k \odot G
$$

展开递推式得到

$$
L_k = \underbrace{G \odot \ldots \odot G}_{k\text{ 次}} = G^{\odot k}
$$

我们仍然可以用矩阵快速幂的方法计算上式，因为它显然是具有结合律的。时间复杂度 $O(n^3 \log k)$。

#### 限长路径计数/最短路

上述算法只适用于边数固定的情况。然而我们可以改进算法以解决边数小于等于 $k$ 的情况。具体地，考虑以下问题：

```admonish question
给一个 $n$ 阶有向图，边权为 $1$，然后给一个整数 $k$，你的任务是对于每个点对 $(u,v)$ 找到从 $u$ 到 $v$ 长度小于等于 $k$ 的路径的数量（不一定是简单路径，即路径上的点或者边可能走多次）。
```

我们对于每个点 $v$，建立一个虚点 $v'$ 用于记录答案，并在图中加入 $(v,v')$ 和 $(v',v')$ 这两条边。那么对于点对 $(u,v)$，从 $u$ 到 $v$ 边数小于等于 $k$ 的路径的数量，就和从 $u$ 到 $v'$ 边数恰好等于 $k+1$ 的路径的数量相等，这是因为对于任意一条边数为 $m(m \le k)$ 的路径 $(p_0=u)\to p_1\to p_2 \to \dots \to p_{m-1} \to (p_m=v)$，都存在一条边数为 $k+1$ 的路径 $(p_0=u)\to p_1 \to p_2 \to \dots \to p_{m-1} \to (p_m=v) \to v'  \to \dots \to v'$ 与之一一对应。

对于求边数小于等于 $k$ 的最短路，只需对每个点加一个边权为 $0$ 的自环即可。

#### 实战演练

```admonish question title = "[P4159 [SCOI2009] 迷路](https://www.luogu.com.cn/problem/P4159)"
该有向图有 $n$ 个节点，节点从 $1$ 至 $n$ 编号，windy 从节点 $1$ 出发，他必须恰好在 $t$ 时刻到达节点 $n$。

现在给出该有向图，你能告诉 windy 总共有多少种不同的路径吗？

答案对 $2009$ 取模。

注意：windy 不能在某个节点逗留，且通过某有向边的时间严格为给定的时间。

- 对于 $30\%$ 的数据，保证 $n \leq 5$，$t \leq 30$。
- 对于 $100\%$ 的数据，保证 $2 \leq n \leq 10$，$1 \leq t \leq 10^9$。
```

与前面不同的是，本题的边有边权

但是边权很小，因此可以拆点

```cpp
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
typedef long long ll;
const ll mod = 2009;
ll n, t;
inline int getid(int x, int y) { return n * y + x; }
inline void add(ll &a, ll b)
{
    a = (a + b) % mod;
}
struct Matrix
{
    int lx, ly;
    ll val[120][120];
    inline void init()
    {
        for (int i = 1; i <= min(lx, ly); i++)
            val[i][i] = 1;
    }
    inline ll *operator[](const int i)
    {
        return val[i];
    }
} e;
Matrix operator*(Matrix a, Matrix b)
{
    Matrix c = {a.lx, b.ly, {}};
    for (int i = 1; i <= a.lx; i++)
        for (int k = 1; k <= a.ly; k++)
            for (int j = 1; j <= b.ly; j++)
                add(c[i][j], a[i][k] * b[k][j] % mod);
    return c;
}
Matrix qpow(Matrix a, ll b)
{
    Matrix c = {a.lx, a.ly, {}};
    for (c.init(); b; b >>= 1ll, a = a * a)
        if (b & 1ll)
            c = c * a;
    return c;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> t;
    e.lx = e.ly = 9 * n;
    char c;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j < 10; j++)
            e[getid(i, j)][getid(i, j - 1)] = 1;
        for (int j = 1; j <= n; j++)
        {
            cin >> c;
            if (c == '0')
                continue;
            e[getid(i, 0)][getid(j, c - '0' - 1)] = 1;
        }
    }
    Matrix g = qpow(e, t);
    cout << g[getid(1, 0)][getid(n, 0)];
    return 0;
}
```

```admonish question title = "[P2151 [SDOI2009] HH去散步](https://www.luogu.com.cn/problem/P2151)"

HH 有个一成不变的习惯，喜欢饭后百步走。所谓百步走，就是散步，就是在一定的时间内，走过一定的距离。但是同时 HH 又是个喜欢变化的人，所以他不会立刻沿着刚刚走来的路走回。又因为 HH 是个喜欢变化的人，所以他每天走过的路径都不完全一样，他想知道他究竟有多少种散步的方法。

现在给你学校的地图（假设每条路的长度都是一样的都是 $1$），问长度为 $t$，从给定地点 $A$ 走到给定地点 $B$ 共有多少条符合条件的路径。

对于 $30\%$ 的数据，$N \le 4$，$M \le 10$，$t \le 10$。

对于 $100\%$ 的数据，$N \le 50$，$M \le 60$，$t \le 2^{30}$，$0 \le A,B$。
```

与前面也有不同，本题不允许连续在两点间来回走动

我们可以将点边互换，如果一条边可以走到另一条边，且这两条边不互为反边，那么就在其对应的点之间两边即可

```cpp
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
typedef long long ll;
const ll mod = 45989, MAXM = 70;
ll n, m, t, a, b, ans;
inline void add(ll &a, ll b)
{
    a = (a + b) % mod;
}
struct Matrix
{
    int lx, ly;
    ll val[MAXM << 1][MAXM << 1];
    inline void init()
    {
        for (int i = 1; i <= min(lx, ly); i++)
            val[i][i] = 1;
    }
    inline ll *operator[](const int i)
    {
        return val[i];
    }
} trans, vec, tvec;
Matrix operator*(Matrix a, Matrix b)
{
    Matrix c = {a.lx, b.ly, {}};
    for (int i = 1; i <= a.lx; i++)
        for (int k = 1; k <= a.ly; k++)
            for (int j = 1; j <= b.ly; j++)
                add(c[i][j], a[i][k] * b[k][j] % mod);
    return c;
}
Matrix qpow(Matrix a, ll b)
{
    Matrix c = {a.lx, a.ly, {}};
    for (c.init(); b; b >>= 1ll, a = a * a)
        if (b & 1ll)
            c = c * a;
    return c;
}
struct edge
{
    int v, nxt;
} e[MAXM << 1];
int head[MAXM << 1], ecnt = 1;
inline void addedge(int u, int v)
{
    e[++ecnt] = {v, head[u]};
    head[u] = ecnt;
}
int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cin >> n >> m >> t >> a >> b;
    if (!t)
        cout << 0, exit(0);
    a++, b++;
    for (int u, v, i = 1; i <= m; i++)
    {
        cin >> u >> v;
        u++, v++;
        addedge(u, v);
        addedge(v, u);
    }
    trans.lx = trans.ly = ecnt;
    for (int i = 2; i <= ecnt; i++)
        for (int j = head[e[i].v]; j; j = e[j].nxt)
            if (j != (i ^ 1))
                trans[j][i]++;
    vec.lx = ecnt, vec.ly = 1;
    for (int i = head[a]; i; i = e[i].nxt)
        vec[i][1]++;
    tvec = qpow(trans, t - 1) * vec;
    for (int i = head[b]; i; i = e[i].nxt)
        add(ans, tvec[i ^ 1][1]);
    cout << ans;
    return 0;
}
```

```admonish question title = "[P6772 [NOI2020] 美食家](https://www.luogu.com.cn/problem/P6772)"
坐落在 Bzeroth 大陆上的精灵王国击退地灾军团的入侵后，经过十余年的休养生息，重新成为了一片欣欣向荣的乐土，吸引着八方游客。小 W 是一位游历过世界各地的著名美食家，现在也慕名来到了精灵王国。

精灵王国共有 $n$ 座城市，城市从 $1$ 到 $n$ 编号，其中城市 $i$ 的美食能为小 W 提供 $c_i$ 的愉悦值。精灵王国的城市通过 $m$ 条**单向道路**连接，道路从 $1$ 到 $m$ 编号，其中道路 $i$ 的起点为城市 $u_i$ ，终点为城市 $v_i$，沿它通行需要花费 $w_i$ 天。也就是说，若小 W 在第 $d$ 天从城市 $u_i$ 沿道路 $i$ 通行，那么他会在第 $d + w_i$ 天到达城市 $v_i$。

小 W 计划在精灵王国进行一场为期 $T$ 天的旅行，更具体地：他会在第 $0$ 天从城市 $1$ 出发，经过 $T$ 天的旅行，最终在**恰好第 $T$ 天**回到城市 $1$ 结束旅行。由于小 W 是一位美食家，每当他到达一座城市时（包括第 $0$ 天和第 $T$ 天的城市 $1$），他都会品尝该城市的美食并获得其所提供的愉悦值，若小 W 多次到达同一座城市，他将**获得多次愉悦值**。注意旅行途中小 W **不能在任何城市停留**，即当他到达一座城市且还未结束旅行时，他当天必须立即从该城市出发前往其他城市。

此外，精灵王国会在**不同**的时间举办 $k$ 次美食节。具体来说，第 $i$ 次美食节将于第 $t_i$ 天在城市 $x_i$ 举办，若小 W 第 $t_i$ 天时恰好在城市 $x_i$，那么他在品尝城市 $x_i$ 的美食时会**额外得到** $y_i$ 的愉悦值。现在小 W 想请作为精灵王国接待使者的你帮他算出，他在旅行中能获得的愉悦值之和的**最大值**。

**若小 W 无法在第 $T$ 天回到城市 $1$，则输出 $-1$**。

本题中数据保证：
- 对所有 $1 \leq i \leq m$，有 $u_i\neq v_i$。但数据中可能存在路线重复的单向道路，即可能存在 $1 \leq i < j \leq m$，使得 $u_i = u_j, v_i = v_j$。
- 对每座城市都满足：至少存在一条以该该城市为起点的单向道路。
- 每次美食节的举办时间 $t_i$ 互不相同。

对于所有测试点：

$1 \leq n \leq 50$，$n \leq m \leq 501$，$0 \leq k \leq 200$，$1 \leq t_i \leq T \leq 10^9$。

$1 \leq w_i \leq 5$，$1 \leq c_i \leq 52501$，$1 \leq u_i, v_i, x_i \leq n$，$1 \leq y_i \leq 10^9$。
```

### 朴素DP
考虑$T$没那么大的时候。我们可以做一个简单的DP。设$f[t][u]$表示在第$t$天，走到了图上的节点$u$，一路上总共能获得的最大愉悦值。转移时，可以枚举点$u$的一条出边$(u, v, w)$，然后用$f[t - 1][u] + w$去更新$f[t][v]$，当然，如果第$t$天点$v$恰好在举办美食节，还要加上额外产生的愉悦值。

因为保证了$t$的递增顺序，所以这个DP满足无后效性，状态非常合理。时间复杂度$O(T \times n \times m)$，期望得分$30$分。加上环的部分分（简单特判），可以得到$40$分。

### 矩阵优化
因为$T$很大而$n, m$都较小，容易想到用矩阵快速幂优化。

对于传统的矩阵乘法，我们是这样定义的：
$$C = A \times B \Leftrightarrow C[i][j] = \sum_{k = 1}^{n}A[i][k] \times B[k][j]$$
但在本题中，DP的转移不是相加而是取$\max$。我们根据本题中的需要，定义一种新的矩阵乘法，不妨记为：
$$C = A \otimes B \Leftrightarrow C[i][j] = \max_{k = 1}^{n}\{A[i][k] + B[k][j]\}$$
也就是把原来外层的$\sum$换成$\max$，把原来内层的$\times$换成$+$。可以证明，这个矩阵乘法仍然是成立的，并且满足原来的种种性质（比如结合律，其实矩阵快速幂优化DP的本质就是用到结合律）。证明略。

值得一提的是，这种重新定义的矩阵乘法，在一类动态DP问题中经常用到。例如NOIP2018保卫王国。

回到我们的DP。先只考虑$K = 0$，也就是没有美食节的情况。

发现从$f[t][u]$转移到$f[t + 1][v]$不太好处理（一般的矩阵快速幂优化DP，只会从$f[t][u]$转移到$f[t + 1][u]$）。但是我们发现$w$很小，$1 \leq w \leq 5$，所以可以考虑把原来的一条边，拆成$5$条边。也就是原本的$(u, v, w)$，拆成：$(u, v, 1), (u, v, 2), (u, v, 3), (u, v, 4), (u, v, 5)$ 。这样虽然点数变多了（变成了$n \times 5$），但是只会从$f[t][u]$转移到$f[t + 1][u]$了，可以用矩阵快速幂优化DP。

具体来说，我们根据两个点之间相连的边权（没有边就是$- \infty$，有边边权就是$w$或$c[v]$，前面已经标出），构造一个转移矩阵。则$f[t] = f[t - 1] \times trans$。答案就是$f[T][1]$。
时间复杂度$O((n \times 5)^3 \times \log T)$。

注意到$T$可能比$n$大不少，所以可以把拆边改成拆点。具体来说，把一个点$u$，变成$u_1, u_2, u_3, u_4, u_5$这样一个结构，边权都是$0$。出发点设为$1_1$。对于一条边$(u, v, w)$，我们从$u_w$向$v_1$连一条边权为$c[v]$的边。这样相当于要先从$u$走到$u_w$，再从$u_w$走到$v_1$，刚好经过了$1$条边，也就是起到了从$f[t][u]$转移到$f[t + 1][v]$的效果。
时间复杂度优化为$O((n \times 5)^2 \times \log T)$。可以通过$60$分的部分分。结合前面的暴力，期望得分$70$分。

### k个美食节怎么处理？
以上的矩阵快速幂，目前还只能处理$K = 0$，也就是没有美食节的情况。考虑$K \gt 0$时怎么做。

注意到题目保证了任意两个美食节不在同一天举办。所以可以先将美食节按举办时间从小到大排序。然后在任意两个美食节之间，做一遍普通的DP转移。例如，第$i$个美食节的举办日期为$t_i$，第$i + 1$个美食节举办日前为$t_{i + 1}$，则这段的转移就是：$f[t_{i + 1}] = f[t_i] \times trans^{t_{i + 1} - t_i}$。转移完成后，再令$f[t_{i + 1}][u_{i + 1}] = \max\{f[t_{i + 1}][u_{i + 1}], f[t_{i + 1}][u_{i + 1}] + w_{i + 1}\}$。也就是加上了美食节的贡献。

最后，如果$t_K \lt T$，再从$f[t_K]$转移到$f[T]$就行。

直接按此做法实现的话，时间复杂度$O(K \times (n \times 5)^2 \times \log T)$。可以通过$80$分的部分分，结合前面的暴力，期望得分$90$分。

### 进一步优化
我们发现所做的$K$次转移，每次都是乘以同一个转移矩阵的若干次幂。

我们考虑把【$trans$的【$2^i$的次幂】次幂】（也就是$trans^{2^i}$）预处理出来。预处理的时间复杂度为$O((n \times 5)^3 \times \log T)$。

然后，在做每个美食节的转移时，对$t_{i + 1} - t_i$这个数做二进制分解，设$t_{i + 1} - t_i = \sum_{j = 0}^{s}a_j2^j$，也就是$a_0, a_1, \cdots, a_s$这些二进制位上为$1$。那么我们只需要用$f[t_i]$，依次乘以预处理好的$trans^{2^j}$（当$a_j = 1$时），即可得到$f[t_{i + 1}]$。而因为$f[t_i]$是一个$1 \times (n \times 5)$的“长条”（又叫向量）而不是一个真正的矩阵，所以每次乘法的时间复杂度只有$O((n \times 5)^2)$。所有转移的总复杂度$O(K \times (n \times 5)^2 \times \log T)$。

总时间复杂度$O((n \times 5)^3 \times \log T + K \times (n \times 5)^2 \times \log T)$。

```cpp
{{#include ./Luogu_P_6772.cpp}}
```