#ifndef MPOLY_HPP
#define MPOLY_HPP
#include <bits/stdc++.h>
using namespace std;
#ifndef rep
#define rep(i, l, r) for (int i = (l); i <= (r); ++i)
#endif
template <int P = 998244353, int G = 3, int ML = 22>
struct Poly : vector<int> {
    using vector<int>::vector;
    using ll = long long;
    using T = Poly<P, G, ML>;
    static constexpr int mod = P;
    static constexpr int root = G;
    static constexpr int ml = ML;
    static_assert(P > 2 && P < (1 << 30));
    static_assert((P - 1LL) % (1LL << ML) == 0);
    static inline int rt[ML + 1];
    static inline int irt[ML + 1];
    static inline int invn[1 << ML];
    static inline int invflg = 1;
    static inline bool inited = false;
    static inline int qpow(int a, long long b) {
        int r = 1;
        for (; b; b >>= 1, a = (ll)a * a % P)
            if (b & 1) r = (ll)r * a % P;
        return r;
    }
    static inline void init() {
        if (inited) return;
        inited = true;
        rep(k, 1, ML) {
            rt[k] = qpow(G, (P - 1LL) >> k);
            irt[k] = qpow(rt[k], P - 2LL);
        }
        invn[1] = 1;
    }
    static inline void initInv(int n) {
        init();
        if (n <= invflg) return;
        n = min(n, P - 1);
        rep(i, invflg + 1, n)
            invn[i] = P - (ll)(P / i) * invn[P % i] % P;
        invflg = n;
    }
    static inline int norm(ll x) {
        x %= P;
        if (x < 0) x += P;
        return (int)x;
    }
    static inline int lim(int n) {
        int z = 1;
        while (z < n) z <<= 1;
        return z;
    }
    static inline void NTT(T &a, bool iv = false) {
        init();
        const int n = (int)a.size();
        if (n <= 1) return;
        if (!iv) {
            for (int len = n, k = __builtin_ctz((unsigned)len); len > 1; len >>= 1, --k) {
                const int half = len >> 1;
                const int wlen = rt[k];
                for (int l = 0; l < n; l += len) {
                    int w = 1;
                    for (int i = 0; i < half; ++i) {
                        const int x = a[l + i];
                        const int y = a[l + i + half];
                        int u = x + y;
                        if (u >= P) u -= P;
                        int v = x - y;
                        if (v < 0) v += P;
                        a[l + i] = u;
                        a[l + i + half] = (ll)v * w % P;
                        w = (ll)w * wlen % P;
                    }
                }
            }
        } else {
            for (int len = 2, k = 1; len <= n; len <<= 1, ++k) {
                const int half = len >> 1;
                const int wlen = irt[k];
                for (int l = 0; l < n; l += len) {
                    int w = 1;
                    for (int i = 0; i < half; ++i) {
                        const int x = a[l + i];
                        const int v = (ll)a[l + i + half] * w % P;
                        int u = x + v;
                        if (u >= P) u -= P;
                        int d = x - v;
                        if (d < 0) d += P;
                        a[l + i] = u;
                        a[l + i + half] = d;
                        w = (ll)w * wlen % P;
                    }
                }
            }
            initInv(n);
            const int iv = invn[n];
            for (int &x : a) x = (ll)x * iv % P;
        }
    }
    static inline void trim(T &a) {
        while (!a.empty() && a.back() == 0) a.pop_back();
    }
    static inline T modXn(T a, int n) {
        if ((int)a.size() > n) a.resize(n);
        return a;
    }
    static inline void read(int n, T &a) {
        a.resize(n);
        for (int &x : a) cin >> x, x = norm(x);
    }
    static inline T read(int n) {
        T a;
        read(n, a);
        return a;
    }
    static inline void write(const T &a) {
        for (int x : a) cout << x << ' ';
        cout << '\n';
    }
    friend inline T operator+(T a, const T &b) {
        if (a.size() < b.size()) a.resize(b.size());
        rep(i, 0, (int)b.size() - 1) {
            int x = a[i] + b[i];
            if (x >= P) x -= P;
            a[i] = x;
        }
        return a;
    }
    friend inline T operator-(T a, const T &b) {
        if (a.size() < b.size()) a.resize(b.size());
        rep(i, 0, (int)b.size() - 1) {
            int x = a[i] - b[i];
            if (x < 0) x += P;
            a[i] = x;
        }
        return a;
    }
    friend inline T operator*(T a, int x) {
        x = norm(x);
        if (x == 0) return T(a.size(), 0);
        if (x == 1) return a;
        for (int &y : a) y = (ll)y * x % P;
        return a;
    }
    friend inline T operator*(int x, T a) { return a * x; }
    friend inline T operator/(T a, int x) {
        x = norm(x);
        assert(x);
        return a * qpow(x, P - 2LL);
    }
    friend inline T operator/(const T &a, const T &b) { return Div(a, b); }
    friend inline T operator%(const T &a, const T &b) { return Mod(a, b); }
    static inline T sqr(const T &a) {
        const int n = (int)a.size();
        if (!n) return {};
        if (n <= 32) {
            T c(2 * n - 1);
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    c[i + j] = (c[i + j] + (ll)a[i] * a[j]) % P;
            return c;
        }
        const int z = lim(2 * n - 1);
        T c(a);
        c.resize(z);
        NTT(c);
        for (int &x : c) x = (ll)x * x % P;
        NTT(c, true);
        c.resize(2 * n - 1);
        return c;
    }
    friend inline T operator*(const T &a, const T &b) {
        const int n = (int)a.size(), m = (int)b.size();
        if (!n || !m) return {};
        if (&a == &b) return sqr(a);
        if (1LL * min(n, m) * max(n, m) <= 4096 || min(n, m) <= 16) {
            T c(n + m - 1);
            if (n < m) {
                for (int i = 0; i < n; ++i)
                    for (int j = 0; j < m; ++j)
                        c[i + j] = (c[i + j] + (ll)a[i] * b[j]) % P;
            } else {
                for (int i = 0; i < m; ++i)
                    for (int j = 0; j < n; ++j)
                        c[i + j] = (c[i + j] + (ll)b[i] * a[j]) % P;
            }
            return c;
        }
        const int z = lim(n + m - 1);
        T A(a), B(b);
        A.resize(z), B.resize(z);
        NTT(A), NTT(B);
        for (int i = 0; i < z; ++i) A[i] = (ll)A[i] * B[i] % P;
        NTT(A, true);
        A.resize(n + m - 1);
        return A;
    }
    static inline T Dert(const T &a) {
        const int n = (int)a.size();
        if (n <= 1) return {};
        T b(n - 1);
        for (int i = 1; i < n; ++i) b[i - 1] = (ll)a[i] * i % P;
        return b;
    }
    static inline T Int(const T &a) {
        const int n = (int)a.size();
        T b(n + 1);
        if (n) {
            initInv(n);
            for (int i = 0; i < n; ++i) b[i + 1] = (ll)a[i] * invn[i + 1] % P;
        }
        return b;
    }
    static inline T Inv(const T &f, int n) {
        assert(!f.empty() && f[0]);
        if (n <= 0) return {};
        if (n == 1) return T{qpow(f[0], P - 2LL)};
        T g{qpow(f[0], P - 2LL)};
        int len = 1;
        while (len < n) {
            const int need = min(n, len << 1);
            const int z = lim(need + (len << 1) - 1);
            T a(f.begin(), f.begin() + min((int)f.size(), need));
            T b(g);
            a.resize(z), b.resize(z);
            NTT(a), NTT(b);
            for (int i = 0; i < z; ++i) {
                int t = b[i] + b[i];
                if (t >= P) t -= P;
                t -= (ll)a[i] * b[i] % P * b[i] % P;
                if (t < 0) t += P;
                b[i] = t;
            }
            NTT(b, true);
            b.resize(need);
            g.swap(b);
            len = need;
        }
        return g;
    }
    static inline T Inv(const T &f) { return Inv(f, (int)f.size()); }
    static inline void Div(const T &f, const T &g, T &q, T &r) {
        assert(!g.empty() && g.back());
        if (f.size() < g.size()) {
            q.clear();
            r = f;
            return;
        }
        const int n = (int)f.size(), m = (int)g.size(), k = n - m + 1;
        T a(f.rbegin(), f.rend()), b(g.rbegin(), g.rend());
        a.resize(k), b.resize(k);
        q = a * Inv(b, k);
        q.resize(k);
        reverse(q.begin(), q.end());
        r = f - g * q;
        r.resize(m - 1);
    }
    static inline T Div(const T &f, const T &g) {
        T q, r;
        Div(f, g, q, r);
        return q;
    }
    static inline T Mod(const T &f, const T &g) {
        T q, r;
        Div(f, g, q, r);
        return r;
    }
    static inline T Ln(const T &f) {
        const int n = (int)f.size();
        assert(n && f[0] == 1);
        if (n == 1) return T{};
        T d = Dert(f);
        d = d * Inv(f, n - 1);
        d.resize(n - 1);
        return Int(d);
    }
    static inline T Exp(const T &f) {
        const int n = (int)f.size();
        assert(n && f[0] == 0);
        if (n == 1) return T{1};
        T g{1};
        for (int len = 1; len < n; len <<= 1) {
            const int need = min(n, len << 1);
            T h(g);
            h.resize(need);
            T lf = Ln(h);
            if ((int)f.size() >= need) {
                for (int i = 0; i < need; ++i) {
                    int x = f[i] - lf[i];
                    if (x < 0) x += P;
                    lf[i] = x;
                }
            } else {
                for (int i = 0; i < (int)f.size(); ++i) {
                    int x = f[i] - lf[i];
                    if (x < 0) x += P;
                    lf[i] = x;
                }
            }
            ++lf[0];
            g = g * lf;
            g.resize(need);
        }
        g.resize(n);
        return g;
    }
    static inline int sqrtx(int a) {
        a = norm(a);
        if (!a) return 0;
        if (qpow(a, (P - 1LL) >> 1) != 1) return -1;
        if ((P & 3) == 3) return qpow(a, (P + 1LL) >> 2);
        int q = P - 1, s = 0;
        while (!(q & 1)) q >>= 1, ++s;
        int z = 2;
        while (qpow(z, (P - 1LL) >> 1) != P - 1) ++z;
        int c = qpow(z, q), x = qpow(a, (q + 1LL) >> 1), t = qpow(a, q);
        int m = s;
        while (t != 1) {
            int i = 1;
            int tt = (ll)t * t % P;
            while (tt != 1) tt = (ll)tt * tt % P, ++i;
            int b = qpow(c, 1LL << (m - i - 1));
            x = (ll)x * b % P;
            t = (ll)t * b % P * b % P;
            c = (ll)b * b % P;
            m = i;
        }
        return x;
    }
    static inline T Sqrt(const T &f) {
        const int n = (int)f.size();
        assert(n);
        int s = sqrtx(f[0]);
        assert(s != -1);
        if (!f[0]) {
            int p = 0;
            while (p < n && !f[p]) ++p;
            if (p == n) return T(n, 0);
            if (p & 1) return {};
            T h(f.begin() + p, f.end());
            h = Sqrt(h);
            T g(n, 0);
            const int sh = p >> 1;
            for (int i = 0; i < (int)h.size() && i + sh < n; ++i) g[i + sh] = h[i];
            return g;
        }
        const int inv2 = (P + 1) >> 1;
        T g{min(s, P - s)};
        for (int len = 1; len < n; len <<= 1) {
            const int need = min(n, len << 1);
            T h(f.begin(), f.begin() + need);
            h = h * Inv(g, need);
            h.resize(need);
            g.resize(need);
            for (int i = 0; i < need; ++i) {
                int x = g[i] + h[i];
                if (x >= P) x -= P;
                g[i] = (ll)x * inv2 % P;
            }
        }
        g.resize(n);
        return g;
    }
    static inline T Pow(T f, long long k) {
        const int n = (int)f.size();
        if (!n) return {};
        if (k == 0) return T(n, 0) + T{1};
        unsigned long long e;
        if (k < 0) {
            f = Inv(f);
            e = 0ULL - (unsigned long long)k;
        } else {
            e = (unsigned long long)k;
        }
        int t = 0;
        while (t < n && f[t] == 0) ++t;
        if (t == n || (unsigned long long)t * e >= (unsigned long long)n) return T(n, 0);
        const int m = n - (int)((unsigned long long)t * e);
        const int c = f[t];
        const int ic = qpow(c, P - 2LL);
        T g(f.begin() + t, f.end());
        for (int &x : g) x = (ll)x * ic % P;
        g.resize(m);
        g = Exp(Ln(g) * (int)(e % P));
        const int ck = qpow(c, e % (P - 1LL));
        T res(n, 0);
        for (int i = 0; i < m; ++i) res[i + t * e] = (ll)g[i] * ck % P;
        return res;
    }
    static inline int iv() {
        static const int i = [] {
            int x = sqrtx(P - 1);
            assert(x != -1);
            return x;
        }();
        return i;
    }
    static inline T Sin(const T &f) {
        const int im = iv();
        const int inv2im = qpow((ll)2 * im % P, P - 2LL);
        return (Exp(f * im) - Exp(f * (P - im))) * inv2im;
    }
    static inline T Cos(const T &f) {
        const int im = iv();
        const int inv2 = (P + 1) >> 1;
        return (Exp(f * im) + Exp(f * (P - im))) * inv2;
    }
    static inline T Asin(const T &f) {
        assert(!f.empty() && f[0] == 0);
        if (f.size() == 1) return T{0};
        T g = T{1} - sqr(f);
        g.resize(f.size());
        g = Inv(Sqrt(g), (int)f.size());
        g = Dert(f) * g;
        g.resize((int)f.size() - 1);
        return Int(g);
    }
    static inline T Atan(const T &f) {
        assert(!f.empty() && f[0] == 0);
        if (f.size() == 1) return T{0};
        T g = T{1} + sqr(f);
        g.resize(f.size());
        g = Inv(g, (int)f.size());
        g = Dert(f) * g;
        g.resize((int)f.size() - 1);
        return Int(g);
    }
};
#ifdef rep
#undef rep
#endif
#endif