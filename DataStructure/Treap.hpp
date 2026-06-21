#pragma once
#include <bits/stdc++.h>
#define int long long
#ifndef rint
#define rint register int
#endif
#ifndef up
#define up(v,s,t,st) for(rint v=s;v<=t;v+=st)
#endif
#ifndef dn
#define dn(v,s,t,st) for(rint v=s;v<t;v+=st)
#endif
#ifndef du
#define du(v,s,t,st) for(rint v=t;v>=s;v-=st)
#endif
#ifndef dl
#define dl(v,s,t,st) for(rint v=t;v>s;v-=st)
#endif
#ifndef ite
#define ite(v, cont) for(const auto& v:cont)
#endif
#ifndef aite
#define aite(v, cont) for(auto &v:cont)
#endif
#ifndef sp
#define sp(t) (!(t&(t-1)))
#endif

using namespace std;

namespace zty
{
    class Treap
    {
    public:
        class Node
        {
        public:
            int nl = 0, nr = 0;
            int num = 0, size = 0;
            unsigned pri = 0;
            int val = 0;
#ifdef ZTY_DEBUG
            void debug(ostream& os) const
            {
                os << "Node(l=" << nl << ", r=" << nr << ", val=" << val
                   << ", num=" << num << ", size=" << size << ", pri=" << pri << ")\n";
            }
#endif
        };

        vector<Node> tree;
        int root = 0, cnt = 0;
        mt19937 rng;

        Treap(int max_size = 200000 + 10) : tree(max_size), rng((unsigned)chrono::steady_clock::now().time_since_epoch().count()) {}

        int New(int val)
        {
            if (cnt + 1 >= (int)tree.size()) tree.resize(tree.size() * 2 + 1);
            tree[++cnt].val = val;
            tree[cnt].pri = rng();
            tree[cnt].num = tree[cnt].size = 1;
            tree[cnt].nl = tree[cnt].nr = 0;
            return cnt;
        }

        void upd(int p)
        {
            if (!p) return;
            tree[p].size = tree[tree[p].nl].size + tree[tree[p].nr].size + tree[p].num;
        }

        void youxuan(int& p)
        {
            int q = tree[p].nl;
            tree[p].nl = tree[q].nr;
            tree[q].nr = p;
            upd(p);
            upd(q);
            p = q;
        }

        void zuoxuan(int& p)
        {
            int q = tree[p].nr;
            tree[p].nr = tree[q].nl;
            tree[q].nl = p;
            upd(p);
            upd(q);
            p = q;
        }

        void Add(int& p, int val)
        {
            if (!p)
            {
                p = New(val);
                return;
            }
            if (tree[p].val == val)
            {
                tree[p].num++;
                upd(p);
                return;
            }
            if (val < tree[p].val)
            {
                Add(tree[p].nl, val);
                if (tree[tree[p].nl].pri > tree[p].pri) youxuan(p);
            }
            else
            {
                Add(tree[p].nr, val);
                if (tree[tree[p].nr].pri > tree[p].pri) zuoxuan(p);
            }
            upd(p);
        }

        void Delete(int& p, int val)
        {
            if (!p) return;
            if (tree[p].val == val)
            {
                if (tree[p].num > 1)
                {
                    tree[p].num--;
                    upd(p);
                    return;
                }
                if (!tree[p].nl || !tree[p].nr)
                {
                    p = tree[p].nl + tree[p].nr;
                    return;
                }
                int sl = tree[p].nl, sr = tree[p].nr;
                if (tree[sl].pri > tree[sr].pri)
                {
                    youxuan(p);
                    Delete(tree[p].nr, val);
                }
                else
                {
                    zuoxuan(p);
                    Delete(tree[p].nl, val);
                }
            }
            else if (val < tree[p].val) Delete(tree[p].nl, val);
            else Delete(tree[p].nr, val);
            upd(p);
        }

        int getidx(int p, int val) const
        {
            if (!p) return 1;
            if (tree[p].val == val) return tree[tree[p].nl].size + 1;
            if (val < tree[p].val) return getidx(tree[p].nl, val);
            return getidx(tree[p].nr, val) + tree[tree[p].nl].size + tree[p].num;
        }

        int getval(int p, int rank) const
        {
            if (!p) return 0;
            int tmp = tree[tree[p].nl].size + tree[p].num;
            int btm = tree[tree[p].nl].size;
            if (rank <= btm) return getval(tree[p].nl, rank);
            if (rank <= tmp) return tree[p].val;
            return getval(tree[p].nr, rank - tmp);
        }

        int Prev(int val) const
        {
            int p = root, res = 0;
            while (p)
            {
                if (tree[p].val < val)
                {
                    res = tree[p].val;
                    p = tree[p].nr;
                }
                else p = tree[p].nl;
            }
            return res;
        }

        int Next(int val) const
        {
            int p = root, res = 0;
            while (p)
            {
                if (tree[p].val > val)
                {
                    res = tree[p].val;
                    p = tree[p].nl;
                }
                else p = tree[p].nr;
            }
            return res;
        }

        void insert(int val) { Add(root, val); }
        void erase(int val) { Delete(root, val); }
        int rank(int val) const { return getidx(root, val); }
        int kth(int rank) const { return getval(root, rank); }
        int prev(int val) const { return Prev(val); }
        int next(int val) const { return Next(val); }
        int size() const { return tree[root].size; }
        bool empty() const { return root == 0; }
        void clear()
        {
            root = cnt = 0;
            fill(tree.begin(), tree.end(), Node());
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "Treap(root=" << root << ", cnt=" << cnt << ", size=" << size() << ")\n";
            for (int i = 1; i <= cnt; i++)
            {
                os << "  [" << i << "] ";
                tree[i].debug(os);
            }
        }
#endif
    };
}
