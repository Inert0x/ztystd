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
    class Trie
    {
    public:
        class Node
        {
        public:
            vector<int> son;
            vector<int> output;
            int cnt = 0;

            Node(int sigma = 26) : son(sigma, 0) {}

#ifdef ZTY_DEBUG
            void debug(ostream& os) const
            {
                os << "TrieNode(cnt=" << cnt << ", output=";
                for (int x : output) os << x << ' ';
                os << ")\n";
            }
#endif
        };

        vector<Node> tree;
        vector<string> words;
        int sigma;
        char base;

        Trie(int sigma = 26, char base = 'a') : sigma(sigma), base(base)
        {
            tree.push_back(Node(sigma));
        }

        int id(char c) const
        {
            int x = c - base;
            return (0 <= x && x < sigma) ? x : -1;
        }

        void clear()
        {
            tree.clear();
            words.clear();
            tree.push_back(Node(sigma));
        }

        int insert(const string& s)
        {
            int p = 0;
            for (char c : s)
            {
                int x = id(c);
                if (x == -1) throw std::invalid_argument("Character is out of Trie alphabet.");
                if (!tree[p].son[x])
                {
                    tree[p].son[x] = tree.size();
                    tree.push_back(Node(sigma));
                }
                p = tree[p].son[x];
            }
            int wid = words.size();
            words.push_back(s);
            tree[p].output.push_back(wid);
            tree[p].cnt++;
            return wid;
        }

        int walk(const string& s) const
        {
            int p = 0;
            for (char c : s)
            {
                int x = id(c);
                if (x == -1 || !tree[p].son[x]) return 0;
                p = tree[p].son[x];
            }
            return p;
        }

        bool contains(const string& s) const
        {
            int p = walk(s);
            return p && tree[p].cnt > 0;
        }

        int count(const string& s) const
        {
            int p = walk(s);
            return p ? tree[p].cnt : 0;
        }

        int size() const { return tree.size(); }
        int word_count() const { return words.size(); }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "Trie(nodes=" << tree.size() << ", words=" << words.size()
               << ", sigma=" << sigma << ", base='" << base << "')\n";
            for (int i = 0; i < (int)tree.size(); i++)
            {
                os << "  [" << i << "] ";
                tree[i].debug(os);
            }
        }
#endif
    };
}
