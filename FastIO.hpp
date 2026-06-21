#pragma once
#include<bits/stdc++.h>
using namespace std;

namespace zty
{
    namespace FastIO 
    {
        const int bufsize = 1 << 20;
        char buf[bufsize], *p1 = buf, *p2 = buf;
        char obuf[bufsize], *O = obuf;
        inline char gc() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, bufsize, stdin), p1 == p2) ? EOF : *p1++; }
        template <typename T> inline void read(T &x) {
            x = 0; T f = 1; char c = gc();
            while (c < '0' || c > '9') { if (c == '-') f = -1; c = gc(); }
            while (c >= '0' && c <= '9') { x = (x << 3) + (x << 1) + (c ^ 48); c = gc(); }
            x *= f;
        }
        inline void flush() { fwrite(obuf, 1, O - obuf, stdout); O = obuf; }
        template <typename T> inline void print(T x) {
            if (x < 0) { *O++ = '-'; x = -x; }
            if (x > 9) print(x / 10);
            *O++ = x % 10 + '0';
        }
        inline void print_char(char c) { *O++ = c; }
    };
};