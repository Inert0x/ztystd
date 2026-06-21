#pragma once
#include <bits/stdc++.h>

using namespace std;

// 这是维护区间和的线段树。请根据具体情况修改。
namespace zty
{
    inline constexpr int segment_tree_maxn = 110;
    inline int ls(int x) { return 2 * x; }
    inline int rs(int x) { return 2 * x + 1; }

    class node
    {
    public:
        int l, r, sum, tag = -1;
#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "node(l=" << l << ", r=" << r << ", sum=" << sum << ", tag=" << tag << ")\n";
        }
#endif
    } tree[segment_tree_maxn];

    class DynamicSegmentTree
    {
    public:
        int cnt;
        DynamicSegmentTree()
        {
            cnt = 1;
        }
        int* lls(int x) { return &tree[x].l; }
        int* rrs(int x) { return &tree[x].r; }
        void pushup(int p) { tree[p].sum = tree[*lls(p)].sum + tree[*rrs(p)].sum; }
        void addtag(int p, int l, int r, int k) // for 区间设置
        {
            tree[p].sum = (r - l + 1) * k;
            tree[p].tag = k;
        }
        void pushdown(int p, int l, int r)
        {
            // 确保调用时左右有定义！
            if (tree[p].tag == -1) return;
            int tg = tree[p].tag;
            tree[p].tag = -1;
            int mid = l + r >> 1;
            addtag(*lls(p), l, mid, tg); addtag(*rrs(p), mid + 1, r, tg);
        }
        void update(int p, int l, int r, int L, int R, int k)
        {
            if (r < L || l > R) return;
            if (L <= l && r <= R) 
            {
                addtag(p, l, r, k);
                return; 
            }
            if (!*lls(p)) *lls(p) = ++cnt;
            if (!*rrs(p)) *rrs(p) = ++cnt;
            pushdown(p, l, r);
            int mid = l + r >> 1;
            update(*lls(p), l, mid, L, R, k); update(*rrs(p), mid + 1, r, L, R, k);
            pushup(p);
        }
        int query(int p, int l, int r, int L, int R)
        {
            if (!p || r < L || l > R) return 0;
            else if (p) if (L <= l && r <= R) return tree[p].sum;
            int mid = l + r >> 1;
            return query(*lls(p), l, mid, L, R) + query(*rrs(p), mid + 1, r, L, R);
        }
#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "DynamicSegmentTree(cnt=" << cnt << ")\n";
            for (int i = 1; i <= cnt; i++)
            {
                os << "  [" << i << "] ";
                tree[i].debug(os);
            }
        }
#endif
    };

    class segtree {
    public:
        vector<int> s, a, tag;
        segtree(vector<int> g) {
            a = g;
            size_t what = g.size();
            what = 4 * what + 40;
            s.resize(what);
            tag.resize(what);
        }
        void pushup(int p) { s[p] = s[ls(p)] + s[rs(p)]; }
        void build(int p, int l, int r) // usage: build(1,1,n)
        {
            if (l == r) {
                s[p] = a[l];
                return;
            }
            int mid = l + r >> 1;
            build(ls(p), l, mid);
            build(rs(p), mid + 1, r);
            pushup(p);
        }
        void addtag(int p, int l, int r, int k) {
            s[p] += (r - l + 1) * k;
            tag[p] += k;
        }
        void pushdown(int p, int l, int r) {
            if (tag[p] == 0)
                return;
            int ctag = tag[p];
            tag[p] = 0;
            int mid = (l + r) >> 1;
            if (l != r) {
                addtag(ls(p), l, mid, ctag);
                addtag(rs(p), mid + 1, r, ctag);
            }
        }
        void update(int p, int l, int r, int L, int R, int k) // usage(1,1,n,L,R,k)
        {
            if (r < L || R < l)
                return;
            if (L <= l && r <= R) {
                addtag(p, l, r, k);
                return;
            }
            pushdown(p, l, r);
            int mid = (l + r) >> 1;
            update(ls(p), l, mid, L, R, k);
            update(rs(p), mid + 1, r, L, R, k);
            pushup(p);
        }
        int ask(int p, int l, int r, int L, int R) // 与 update 类似
        {
            if (r < L || R < l)
                return 0;
            if (L <= l && r <= R) {
                return s[p];
            }
            pushdown(p, l, r);
            int mid = (l + r) >> 1;
            int lans = ask(ls(p), l, mid, L, R);
            int rans = ask(rs(p), mid + 1, r, L, R);
            return lans + rans;
        }
#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "segtree(size=" << a.size() << ", storage=" << s.size() << ")\n";
            os << "a:";
            for (size_t i = 0; i < a.size(); i++) os << ' ' << a[i];
            os << "\ns:";
            for (size_t i = 0; i < s.size(); i++) os << ' ' << s[i];
            os << "\ntag:";
            for (size_t i = 0; i < tag.size(); i++) os << ' ' << tag[i];
            os << '\n';
        }
#endif
    };
}
