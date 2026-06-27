#pragma once
#include <bits/stdc++.h>

using namespace std;

namespace zty
{
    class Prime
    {
    public:
        vector<int> primes;
        vector<int> min_factor;
        vector<int> phi;
        vector<char> is_prime_table;

        Prime() {}
        Prime(int n, bool build_phi = false)
        {
            linear_sieve(n, build_phi);
        }

        vector<int> eratosthenes(int n)
        {
            primes.clear();
            is_prime_table.assign(n + 1, true);
            if (n >= 0) is_prime_table[0] = false;
            if (n >= 1) is_prime_table[1] = false;
            for (int i = 2; i <= n; i++)
            {
                if (is_prime_table[i]) primes.push_back(i);
                if ((long long)i * i > n) continue;
                if (!is_prime_table[i]) continue;
                for (long long j = (long long)i * i; j <= n; j += i) is_prime_table[(int)j] = false;
            }
            return primes;
        }

        vector<int> linear_sieve(int n, bool build_phi = false)
        {
            primes.clear();
            min_factor.assign(n + 1, 0);
            is_prime_table.assign(n + 1, false);
            if (build_phi) phi.assign(n + 1, 0);
            if (n >= 1 && build_phi) phi[1] = 1;
            for (int i = 2; i <= n; i++)
            {
                if (!min_factor[i])
                {
                    min_factor[i] = i;
                    primes.push_back(i);
                    is_prime_table[i] = true;
                    if (build_phi) phi[i] = i - 1;
                }
                for (int p : primes)
                {
                    long long v = (long long)i * p;
                    if (v > n || p > min_factor[i]) break;
                    min_factor[(int)v] = p;
                    if (build_phi)
                    {
                        if (i % p == 0) phi[(int)v] = phi[i] * p;
                        else phi[(int)v] = phi[i] * (p - 1);
                    }
                    if (i % p == 0) break;
                }
            }
            return primes;
        }

        vector<int> phi_array(int n)
        {
            linear_sieve(n, true);
            return phi;
        }

        bool is_prime(long long x) const
        {
            if (x < 2) return false;
            if (x < (long long)is_prime_table.size()) return is_prime_table[(int)x];
            for (long long p : primes)
            {
                if (p * p > x) break;
                if (x % p == 0) return false;
            }
            long long start = primes.empty() ? 2 : primes.back() + 1LL;
            for (long long i = start; i * i <= x; i++)
                if (x % i == 0) return false;
            return true;
        }

        vector<long long> divisors(long long x) const
        {
            vector<long long> res;
            for (long long i = 1; i * i <= x; i++)
            {
                if (x % i) continue;
                res.push_back(i);
                if (i * i != x) res.push_back(x / i);
            }
            sort(res.begin(), res.end());
            return res;
        }

        vector<pair<long long, int>> factorize(long long x) const
        {
            vector<pair<long long, int>> res;
            if (x < 0) x = -x;
            for (long long p : primes)
            {
                if (p * p > x) break;
                if (x % p) continue;
                int cnt = 0;
                while (x % p == 0)
                {
                    x /= p;
                    cnt++;
                }
                res.push_back({p, cnt});
            }
            for (long long p = primes.empty() ? 2 : primes.back() + 1LL; p * p <= x; p++)
            {
                if (x % p) continue;
                int cnt = 0;
                while (x % p == 0)
                {
                    x /= p;
                    cnt++;
                }
                res.push_back({p, cnt});
            }
            if (x > 1) res.push_back({x, 1});
            return res;
        }

        long long euler_phi(long long x) const
        {
            if (x <= 0) return 0;
            long long res = x;
            vector<pair<long long, int>> fac = factorize(x);
            for (auto [p, cnt] : fac) res = res / p * (p - 1);
            return res;
        }

        vector<int> prime_prefix_count(int n)
        {
            if ((int)is_prime_table.size() <= n) linear_sieve(n);
            vector<int> res(n + 1, 0);
            for (int i = 1; i <= n; i++) res[i] = res[i - 1] + (is_prime_table[i] ? 1 : 0);
            return res;
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "Prime(primes=" << primes.size() << ", table_size="
               << (is_prime_table.empty() ? 0 : (int)is_prime_table.size() - 1) << ")\n";
            os << "primes:";
            for (int p : primes) os << ' ' << p;
            os << '\n';
        }
#endif
    };

    inline vector<int> Eratosthenes(int n)
    {
        Prime p;
        return p.eratosthenes(n);
    }

    inline vector<int> LinearSieve(int n)
    {
        Prime p;
        return p.linear_sieve(n);
    }

    inline vector<int> PhiArray(int n)
    {
        Prime p;
        return p.phi_array(n);
    }

    inline bool IsPrime(long long x)
    {
        Prime p((int)sqrt((long double)x) + 1);
        return p.is_prime(x);
    }

    inline vector<long long> Divisors(long long x)
    {
        Prime p;
        return p.divisors(x);
    }

    inline vector<pair<long long, int>> Factorize(long long x)
    {
        Prime p((int)sqrt((long double)llabs(x)) + 1);
        return p.factorize(x);
    }

    inline long long EulerPhi(long long x)
    {
        Prime p((int)sqrt((long double)x) + 1);
        return p.euler_phi(x);
    }
}
