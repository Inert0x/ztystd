#pragma once
#include <bits/stdc++.h>

using namespace std;

namespace zty
{
    inline long long exgcd(long long a, long long b, long long& x, long long& y)
    {
        if (b == 0)
        {
            x = (a >= 0 ? 1 : -1);
            y = 0;
            return llabs(a);
        }
        long long x1, y1;
        long long g = exgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - a / b * y1;
        return g;
    }

    inline long long qpow(long long a, long long b, long long MOD)
    {
        if (MOD <= 0) throw std::invalid_argument("MOD must be positive.");
        a %= MOD;
        if (a < 0) a += MOD;
        long long res = 1 % MOD;
        while (b)
        {
            if (b & 1) res = (long long)((__int128)res * a % MOD);
            a = (long long)((__int128)a * a % MOD);
            b >>= 1;
        }
        return res;
    }

    inline long long Inv(long long a, long long MOD)
    {
        if (MOD <= 0) throw std::invalid_argument("MOD must be positive.");
        long long x, y;
        long long g = exgcd(a, MOD, x, y);
        if (g != 1) throw std::invalid_argument("Inverse does not exist.");
        x %= MOD;
        if (x < 0) x += MOD;
        return x;
    }

    class ModInt
    {
    public:
        inline static long long MOD = 1000000007LL;
        long long val;

        static void SetMod(long long mod)
        {
            if (mod <= 0) throw std::invalid_argument("MOD must be positive.");
            MOD = mod;
        }

        ModInt(long long v = 0)
        {
            val = v % MOD;
            if (val < 0) val += MOD;
        }

        long long value() const { return val; }
        ModInt inv() const { return ModInt(Inv(val, MOD)); }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "ModInt(value=" << val << ", MOD=" << MOD << ")\n";
        }
#endif

        ModInt& operator+=(const ModInt& other)
        {
            val += other.val;
            if (val >= MOD) val -= MOD;
            return *this;
        }
        ModInt& operator-=(const ModInt& other)
        {
            val -= other.val;
            if (val < 0) val += MOD;
            return *this;
        }
        ModInt& operator*=(const ModInt& other)
        {
            val = (long long)((__int128)val * other.val % MOD);
            return *this;
        }
        ModInt& operator/=(const ModInt& other)
        {
            return *this *= other.inv();
        }

        friend ModInt operator+(ModInt a, const ModInt& b) { return a += b; }
        friend ModInt operator-(ModInt a, const ModInt& b) { return a -= b; }
        friend ModInt operator*(ModInt a, const ModInt& b) { return a *= b; }
        friend ModInt operator/(ModInt a, const ModInt& b) { return a /= b; }
        friend bool operator==(const ModInt& a, const ModInt& b) { return a.val == b.val; }
        friend bool operator!=(const ModInt& a, const ModInt& b) { return !(a == b); }
        friend ostream& operator<<(ostream& os, const ModInt& a) { return os << a.val; }
        friend istream& operator>>(istream& is, ModInt& a)
        {
            long long x;
            is >> x;
            a = ModInt(x);
            return is;
        }
    };

    inline ModInt qpow(ModInt a, long long b)
    {
        ModInt res(1);
        while (b)
        {
            if (b & 1) res *= a;
            a *= a;
            b >>= 1;
        }
        return res;
    }
}
