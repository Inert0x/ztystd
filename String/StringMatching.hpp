#pragma once
#include <bits/stdc++.h>
#include "Trie.hpp"

using namespace std;

namespace zty
{
    class KMP
    {
    public:
        string pattern;
        vector<int> nxt;

        KMP() {}
        KMP(const string& s) { build(s); }

        void build(const string& s)
        {
            pattern = s;
            nxt.assign(pattern.size(), 0);
            for (int i = 1, j = 0; i < (int)pattern.size(); i++)
            {
                while (j && pattern[i] != pattern[j]) j = nxt[j - 1];
                if (pattern[i] == pattern[j]) j++;
                nxt[i] = j;
            }
        }

        vector<int> match(const string& text) const
        {
            vector<int> res;
            if (pattern.empty()) return res;
            for (int i = 0, j = 0; i < (int)text.size(); i++)
            {
                while (j && text[i] != pattern[j]) j = nxt[j - 1];
                if (text[i] == pattern[j]) j++;
                if (j == (int)pattern.size())
                {
                    res.push_back(i - (int)pattern.size() + 1);
                    j = nxt[j - 1];
                }
            }
            return res;
        }

        int count(const string& text) const
        {
            return (int)match(text).size();
        }

        int first_pos(const string& text) const
        {
            vector<int> res = match(text);
            return res.empty() ? -1 : res[0];
        }

        static vector<int> prefix_function(const string& s)
        {
            vector<int> pi(s.size(), 0);
            for (int i = 1, j = 0; i < (int)s.size(); i++)
            {
                while (j && s[i] != s[j]) j = pi[j - 1];
                if (s[i] == s[j]) j++;
                pi[i] = j;
            }
            return pi;
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "KMP(pattern=\"" << pattern << "\", length=" << pattern.size() << ")\n";
            os << "nxt:";
            for (int x : nxt) os << ' ' << x;
            os << '\n';
        }
#endif
    };

    class ACAutomaton
    {
    public:
        Trie trie;
        vector<int> fail;
        int sigma;
        char base;
        bool built = false;

        ACAutomaton(int sigma = 26, char base = 'a') : sigma(sigma), base(base)
        {
            trie = Trie(sigma, base);
            fail.assign(1, 0);
        }

        int id(char c) const
        {
            return trie.id(c);
        }

        void clear()
        {
            trie.clear();
            fail.assign(1, 0);
            built = false;
        }

        int insert(const string& s)
        {
            if (built) throw std::invalid_argument("Cannot insert after ACAutomaton has been built. Call clear() first.");
            return trie.insert(s);
        }

        void build()
        {
            if (built) return;
            fail.assign(trie.tree.size(), 0);
            queue<int> q;
            for (int i = 0; i < sigma; i++)
            {
                int v = trie.tree[0].son[i];
                if (v) q.push(v);
            }
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                int f = fail[u];
                for (int x : trie.tree[f].output) trie.tree[u].output.push_back(x);
                trie.tree[u].cnt += trie.tree[f].cnt;
                for (int i = 0; i < sigma; i++)
                {
                    int v = trie.tree[u].son[i];
                    if (v)
                    {
                        fail[v] = trie.tree[fail[u]].son[i];
                        q.push(v);
                    }
                    else trie.tree[u].son[i] = trie.tree[fail[u]].son[i];
                }
            }
            built = true;
        }

        vector<pair<int, int>> match(const string& text)
        {
            if (!built) build();
            vector<pair<int, int>> res;
            int p = 0;
            for (int i = 0; i < (int)text.size(); i++)
            {
                int x = id(text[i]);
                if (x == -1)
                {
                    p = 0;
                    continue;
                }
                p = trie.tree[p].son[x];
                for (int pid : trie.tree[p].output)
                {
                    res.push_back({i - (int)trie.words[pid].size() + 1, pid});
                }
            }
            return res;
        }

        vector<int> count_each(const string& text)
        {
            vector<int> res(trie.words.size(), 0);
            vector<pair<int, int>> matches = match(text);
            for (auto p : matches) res[p.second]++;
            return res;
        }

        int count(const string& text)
        {
            if (!built) build();
            int res = 0, p = 0;
            for (char c : text)
            {
                int x = id(c);
                if (x == -1)
                {
                    p = 0;
                    continue;
                }
                p = trie.tree[p].son[x];
                res += trie.tree[p].cnt;
            }
            return res;
        }

        int size() const { return trie.size(); }
        int pattern_count() const { return trie.word_count(); }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "ACAutomaton(nodes=" << trie.size() << ", patterns=" << trie.words.size()
               << ", sigma=" << sigma << ", base='" << base << "', built=" << built << ")\n";
            for (int i = 0; i < (int)trie.tree.size(); i++)
            {
                os << "  [" << i << "] fail=" << (i < (int)fail.size() ? fail[i] : 0) << ' ';
                trie.tree[i].debug(os);
            }
        }
#endif
    };
}
