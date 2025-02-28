# pbds 库学习笔记

### 一、前言
pbds 库即 “Policy - Based Data Structures”，是基于策略的数据结构，它提供了多种强大的数据结构，如 “优先队列”、“平衡树”、“哈希表” 等，这些数据结构在算法竞赛中具有广泛的应用。与 STL（Standard Template Library）相比，pbds 库在处理某些特定问题时效率更高，功能更强大。本文综合多个学习资源，旨在为 OI 领域的选手提供一份全面且实用的 pbds 库学习资料。

### 二、概述
pbds 库的主要组成部分包括：
- “优先队列”（priority_queue）：支持高效的堆操作，如 `push`、`pop`、`top`，并且额外提供了 `modify` 和 `join` 等高级操作。
- “平衡树”（tree）：提供了丰富的操作，如插入、删除、查找、求排名、求第 k 小等，功能远超 STL 中的 `set`。
- “哈希表”（hash_table）：提供了两种哈希表实现，分别是基于拉链法的 `cc_hash_table` 和基于探测法的 `gp_hash_table`，性能出色。

使用 pbds 库需要包含特定的头文件：
~~~cpp
#include <bits/extc++.h>
~~~
此外，为了避免命名冲突，需要使用以下命名空间：
~~~cpp
using namespace __gnu_cxx;
using namespace __gnu_pbds;
~~~
当然，vscode中使用上面方法有问题，也可以这么做来代替：
~~~cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/hash_policy.hpp>
namespace pbds = __gnu_pbds;
~~~

### 三、priority_queue（优先队列）
#### 3.1 概述
pbds 的优先队列与 STL 中的优先队列类似，但功能更强大。它支持以下操作：
- 基本操作：`push`、`pop`、`top`、`empty`、`size`、`clear`。
- 高级操作：`modify`（修改堆中元素）、`join`（合并两个堆）。

优先队列的声明形式如下：
~~~cpp
__gnu_pbds::priority_queue<T, Cmp, Tag> q;
~~~
- `T`：元素类型。
- `Cmp`：比较函数，如 `greater<T>`（小根堆）、`less<T>`（大根堆）。
- `Tag`：堆的类型，如 `pairing_heap_tag`（配对堆）、`thin_heap_tag`（斐波那契堆）。

通常推荐使用 `pairing_heap_tag`，因为它在大多数情况下性能较好。

#### 3.2 基本操作示例
##### 3.2.1 声明与初始化
~~~cpp
typedef pair<int, int> PII;
__gnu_pbds::priority_queue<PII, greater<PII>, pairing_heap_tag> q;
~~~
##### 3.2.2 基本操作
~~~cpp
q.push({3, 5}); // 将元素 {3, 5} 压入堆中
q.push({1, 2});
q.push({2, 4});

cout << q.top().first << " " << q.top().second << endl; // 输出堆顶元素
q.pop();                                                  // 弹出堆顶元素

cout << q.size() << endl;  // 输出堆的大小
cout << q.empty() << endl; // 判断堆是否为空
~~~
##### 3.2.3 清空操作
~~~cpp
q.clear(); // 清空堆
~~~
#### 3.3 modify 操作
`modify` 操作用于修改堆中的元素。`push` 操作会返回一个迭代器，通过这个迭代器可以修改元素。
~~~cpp
__gnu_pbds::priority_queue<PII, greater<PII>, pairing_heap_tag>::point_iterator it = q.push({3, 5});
q.modify(it, {1, 2}); // 将迭代器 it 指向的元素修改为 {1, 2}
~~~
#### 3.4 join 操作
`join` 操作用于合并两个堆。合并后，被合并的堆会被清空。
~~~cpp
__gnu_pbds::priority_queue<PII, greater<PII>, pairing_heap_tag> q1, q2;
q1.push({1, 2});
q2.push({3, 4});
q2.push({3, 6});
q2.push({1, 5});

q1.join(q2); // 将 q2 合并到 q1 中，q2 被清空
~~~
#### 3.5 Dijkstra 最短路径示例
在 Dijkstra 算法中，优先队列的 `modify` 操作可以高效地更新节点的最短距离。
~~~cpp
typedef pair<int, int> PII;
__gnu_pbds::priority_queue<PII, greater<PII>, pairing_heap_tag> q;
__gnu_pbds::priority_queue<PII, greater<PII>, pairing_heap_tag>::point_iterator its[N];
int dis[N];
vector<PII> g[N];

void dijkstra(int sta) {
    q.clear();
    memset(dis, 0x3f, sizeof dis);
    its[sta] = q.push({0, sta});
    dis[sta] = 0;
    int u;
    #define v e.first
    #define w e.second
    while (!q.empty()) {
        u = q.top().second;
        q.pop();
        for (auto e : g[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (its[v]!= NULL) {
                    q.modify(its[v], {dis[v], v});
                } else {
                    its[v] = q.push({dis[v], v});
                }
            }
        }
    }
    #undef v
    #undef w
}
~~~

### 四、tree（平衡树）
#### 4.1 概述
pbds 的平衡树提供了丰富的操作，如插入、删除、查找、求排名、求第 k 小等。其声明形式如下：
~~~cpp
tree<Key, Mapped, Cmp_Fn, Tag, Node_Update> tr;
~~~
- `Key`：键的类型。
- `Mapped`：值的类型，若为 `null_type`，表示该树类似 `set`；否则类似 `map`。
- `Cmp_Fn`：比较函数，如 `less<Key>`（升序）、`greater<Key>`（降序）。
- `Tag`：平衡树的类型，如 `rb_tree_tag`（红黑树）、`splay_tree_tag`（伸展树）。
- `Node_Update`：节点更新策略，常用 `tree_order_statistics_node_update`，用于支持 `order_of_key` 和 `find_by_order`。

#### 4.2 基本操作
##### 4.2.1 插入与删除
~~~cpp
tr.insert(x); // 插入元素 x
tr.erase(x);  // 删除元素 x
~~~
##### 4.2.2 查找
- `find_by_order(k)`：返回排名第 $k$ 的元素（$0$ - based）。
- `order_of_key(x)`：返回 $x$ 的排名（比 $x$ 小的元素个数）。
- `lower_bound(x)`：返回大于等于 $x$ 的最小元素迭代器。
- `upper_bound(x)`：返回大于 $x$ 的最小元素迭代器。

##### 4.2.3 合并与分裂
- `tr1.join(tr2)`：将 `tr2` 合并到 `tr1` 中，`tr2` 被清空。
- `tr1.split(x, tr2)`：将 `tr1` 中小于等于 $x$ 的元素分割到 `tr1` 中，其余分割到 `tr2` 中。

#### 4.3 示例代码
##### 4.3.1 普通平衡树操作
~~~cpp
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> Tree;
Tree tr;

tr.insert(5);
tr.insert(6);
tr.insert(7);

cout << *tr.find_by_order(1) << endl; // 输出 5
cout << tr.order_of_key(5) << endl;   // 输出 1
~~~
##### 4.3.2 合并与分裂
~~~cpp
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> Tree;
Tree a, b;

a.insert(1);
a.insert(5);
b.insert(6);
b.insert(7);

a.join(b); // 合并 a 和 b，b 被清空
for (int x : a) {
    cout << x << " "; // 输出 1 5 6 7
}
cout << endl;

a.split(5, b); // 分裂 a，小于等于 5 的留在 a 中，其余到 b 中
for (int x : a) {
    cout << x << " "; // 输出 1 5
}
cout << endl;
for (int x : b) {
    cout << x << " "; // 输出 6 7
}
cout << endl;
~~~
#### 4.4 例题：洛谷 P3369 普通平衡树
题目要求维护一个平衡树，支持以下操作：
- 插入 $x$。
- 删除 $x$（若存在多个相同的数，只删除一个）。
- 查询 $x$ 的排名。
- 查询排名为 $x$ 的数。
- 查询 $x$ 的前驱。
- 查询 $x$ 的后继。

解决方法：
- 使用 `pair<int, int>` 存储元素，`second` 表示插入时间，避免重复。
- `order_of_key` 和 `find_by_order` 函数用于处理排名和第 $k$ 小。

代码示例：
~~~cpp
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

typedef pair<int, int> PII;
typedef tree<PII, null_type, less<PII>, rb_tree_tag, tree_order_statistics_node_update> Tree;
Tree tr;

int main() {
    int n;
    cin >> n;
    int tr_clock = 0;
    while (n--) {
        int op, x;
        cin >> op >> x;
        tr_clock++;
        switch (op) {
            case 1:
                tr.insert(PII(x, tr_clock));
                break;
            case 2:
                tr.erase(PII(x, 0)); // 删除第一个大于等于 (x,0) 的元素
                break;
            case 3:
                cout << tr.order_of_key(PII(x, 0)) + 1 << endl;
                break;
            case 4:
                cout << tr.find_by_order(x - 1)->first << endl;
                break;
            case 5:
                cout << (--tr.upper_bound(PII(x, 0)))->first << endl;
                break;
            case 6:
                cout << tr.upper_bound(PII(x, INT_MAX))->first << endl;
                break;
        }
    }
    return 0;
}
~~~

### 五、hash_table（哈希表）
#### 5.1 概述
pbds 提供了两种哈希表：
- `cc_hash_table<Key, Mapped>`：基于拉链法的哈希表，适合一般情况。
- `gp_hash_table<Key, Mapped>`：基于探测法的哈希表，性能更优，推荐使用。

哈希表的声明形式如下：
~~~cpp
__gnu_pbds::gp_hash_table<Key, Mapped> hs;
~~~
- `Key`：键的类型。
- `Mapped`：值的类型。

#### 5.2 基本操作
##### 5.2.1 插入与查询
~~~cpp
hs[key] = value; // 插入或更新键值对
if (hs.find(key)!= hs.end()) {
    // 键存在
}
~~~
##### 5.2.2 删除
~~~cpp
hs.erase(key); // 删除键值对
~~~
#### 5.3 示例代码
~~~cpp
__gnu_pbds::gp_hash_table<string, int> hs;

hs["apple"] = 2;
hs["banana"] = 3;

if (hs.find("apple")!= hs.end()) {
    cout << "apple exists" << endl;
}

hs.erase("banana");
~~~
#### 5.4 与 STL 的对比
- pbds 的哈希表性能优于 STL 的 `unordered_map`，尤其是在大规模数据下。
- pbds 的哈希表不支持排序，遍历时是无序的。

### 六、性能优化
#### 6.1 编译器优化
在使用 pbds 库时，编译器的优化选项对性能有显著影响。建议使用 `-O2` 或 `-O3` 优化选项进行编译。

#### 6.2 选择合适的数据结构
根据具体问题选择合适的数据结构可以显著提高性能。例如：
- 对于需要频繁合并和分裂的场景，优先选择 `pairing_heap_tag`。
- 对于需要快速查找和插入的场景，优先选择 `rb_tree_tag`。

#### 6.3 优化哈希函数
对于哈希表，优化哈希函数可以减少哈希冲突，提高性能。例如，可以自定义哈希函数：
~~~cpp
template <> struct std::tr1::hash<pair<int, int>> {
    size_t operator()(const pair<int, int>& x) const {
        return x.first ^ x.second;
    }
};
~~~

### 七、实际应用案例
#### 7.1 洛谷 P2580 于是他错误的点名开始了
题目要求统计字符串的出现次数，并根据不同的操作输出相应的结果。使用 `gp_hash_table` 可以高效地完成这一任务。
~~~cpp
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

int main() {
    int n, m;
    cin >> n;
    gp_hash_table<string, int> hs;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        hs[s]++;
    }
    cin >> m;
    for (int i = 0; i < m; i++) {
        string s;
        cin >> s;
        if (hs[s] > 0) {
            cout << "OK" << endl;
            hs[s]--;
        } else {
            cout << "WRONG" << endl;
        }
    }
    return 0;
}
~~~
#### 7.2 洛谷 P3369 普通平衡树
题目要求维护一个平衡树，支持多种操作。使用 `tree` 可以高效地完成这一任务。
~~~cpp
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

typedef pair<int, int> PII;
typedef tree<PII, null_type, less<PII>, rb_tree_tag, tree_order_statistics_node_update> Tree;
Tree tr;

int main() {
    int n;
    cin >> n;
    int tr_clock = 0;
    while (n--) {
        int op, x;
        cin >> op >> x;
        tr_clock++;
        switch (op) {
            case 1:
                tr.insert(PII(x, tr_clock));
                break;
            case 2:
                tr.erase(PII(x, 0));
                break;
            case 3:
                cout << tr.order_of_key(PII(x, 0)) + 1 << endl;
                break;
            case 4:
                cout << tr.find_by_order(x - 1)->first << endl;
                break;
            case 5:
                cout << (--tr.upper_bound(PII(x, 0)))->first << endl;
                break;
            case 6:
                cout << tr.upper_bound(PII(x, INT_MAX))->first << endl;
                break;
        }
    }
    return 0;
}
~~~

### 八、参考
- “C++ pb_ds 食用教程”
- “魔法之 pb_ds”
- “NOI系列竞赛可用的非主流技巧”
- “OI入门算法详解：含大量优质习题及题解！”