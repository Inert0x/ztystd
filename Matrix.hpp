#pragma once
#include <bits/stdc++.h>
#include "Moding.hpp"

using namespace std;

namespace zty
{
    class Matrix
    {
    public:
        vector<vector<double>> mat;
        int row,col;
        Matrix(int n, int m)
        {
            mat.resize(n, vector<double>(m, 0));
            row=n,col=m;
        }
        Matrix(int n, int m, int val)
        {
            mat.resize(n,vector<double>(m,val));
            row=n,col=m;
        }
        vector<double>& operator[](int i){return mat[i];}
        const vector<double>& operator[](int i) const { return mat[i]; }
#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "Matrix(row=" << row << ", col=" << col << ")\n";
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++) os << mat[i][j] << (j + 1 == col ? '\n' : ' ');
            }
        }
#endif
        double determinant() const
        {
            if (row != col) throw std::invalid_argument("Only square matrices have determinants.");
            const double eps = 1e-12;
            Matrix tmp = *this;
            double res = 1;
            for (int i = 0; i < row; i++)
            {
                int pivot = i;
                for (int j = i + 1; j < row; j++)
                    if (fabs(tmp[j][i]) > fabs(tmp[pivot][i])) pivot = j;
                if (fabs(tmp[pivot][i]) < eps) return 0;
                if (pivot != i)
                {
                    swap(tmp[pivot], tmp[i]);
                    res = -res;
                }
                res *= tmp[i][i];
                for (int j = i + 1; j < row; j++)
                {
                    double ratio = tmp[j][i] / tmp[i][i];
                    for (int k = i; k < row; k++) tmp[j][k] -= ratio * tmp[i][k];
                }
            }
            return res;
        }
        Matrix inverse() const
        {
            if (row != col) throw std::invalid_argument("Only square matrices can be inversed.");
            const double eps = 1e-12;
            Matrix a = *this;
            Matrix res(row, row);
            for (int i = 0; i < row; i++) res[i][i] = 1;
            for (int i = 0; i < row; i++)
            {
                int pivot = i;
                for (int j = i + 1; j < row; j++)
                    if (fabs(a[j][i]) > fabs(a[pivot][i])) pivot = j;
                if (fabs(a[pivot][i]) < eps) throw std::invalid_argument("Singular matrix has no inverse.");
                if (pivot != i)
                {
                    swap(a[pivot], a[i]);
                    swap(res[pivot], res[i]);
                }
                double div = a[i][i];
                for (int j = 0; j < row; j++)
                {
                    a[i][j] /= div;
                    res[i][j] /= div;
                }
                for (int j = 0; j < row; j++)
                {
                    if (j == i) continue;
                    double ratio = a[j][i];
                    for (int k = 0; k < row; k++)
                    {
                        a[j][k] -= ratio * a[i][k];
                        res[j][k] -= ratio * res[i][k];
                    }
                }
            }
            return res;
        }
    };
    inline Matrix operator*(const Matrix& a, const Matrix& b)
    {
        int arow=a.row, acol=a.col;
        int brow=b.row, bcol=b.col;
        Matrix res(arow,bcol);
        if (acol != brow) throw std::invalid_argument("Two matrices are not able to product.");
        for(int i=0;i<arow;i++) for(int j=0;j<bcol;j++) for(int k=0;k<acol;k++) res[i][j]+=a[i][k]*b[k][j];
        return res;
    }
    inline Matrix GetIdenticalMatrix(int n)
    {
        Matrix res(n,n);
        for(int i=0;i<n;i++) res[i][i]=1;
        return res;
    }
    inline double Det(const Matrix& a) { return a.determinant(); }
    inline Matrix Inverse(const Matrix& a) { return a.inverse(); }

    class ModMatrix
    {
    public:
        vector<vector<ModInt>> mat;
        long long MOD;
        int row, col;

        ModMatrix(int n, int m)
        {
            mat.resize(n, vector<ModInt>(m, 0));
            MOD = ModInt::MOD;
            row = n, col = m;
        }
        ModMatrix(int n, int m, long long mod)
        {
            ModInt::SetMod(mod);
            mat.resize(n, vector<ModInt>(m, 0));
            MOD = mod;
            row = n, col = m;
        }
        ModMatrix(int n, int m, long long mod, long long val)
        {
            ModInt::SetMod(mod);
            mat.resize(n, vector<ModInt>(m, val));
            MOD = mod;
            row = n, col = m;
        }

        vector<ModInt>& operator[](int i) { return mat[i]; }
        const vector<ModInt>& operator[](int i) const { return mat[i]; }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "ModMatrix(row=" << row << ", col=" << col << ", MOD=" << MOD << ")\n";
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++) os << mat[i][j] << (j + 1 == col ? '\n' : ' ');
            }
        }
#endif

        ModInt determinant() const
        {
            if (row != col) throw std::invalid_argument("Only square matrices have determinants.");
            ModInt::SetMod(MOD);
            ModMatrix tmp = *this;
            ModInt res(1);
            for (int i = 0; i < row; i++)
            {
                int pivot = -1;
                for (int j = i; j < row; j++)
                {
                    if (tmp[j][i].value() != 0)
                    {
                        pivot = j;
                        break;
                    }
                }
                if (pivot == -1) return ModInt(0);
                if (pivot != i)
                {
                    swap(tmp[pivot], tmp[i]);
                    res *= ModInt(-1);
                }
                res *= tmp[i][i];
                ModInt inv = tmp[i][i].inv();
                for (int j = i + 1; j < row; j++)
                {
                    ModInt ratio = tmp[j][i] * inv;
                    for (int k = i; k < row; k++) tmp[j][k] -= ratio * tmp[i][k];
                }
            }
            return res;
        }

        ModMatrix inverse() const
        {
            if (row != col) throw std::invalid_argument("Only square matrices can be inversed.");
            ModInt::SetMod(MOD);
            ModMatrix a = *this;
            ModMatrix res(row, row, MOD);
            for (int i = 0; i < row; i++) res[i][i] = 1;
            for (int i = 0; i < row; i++)
            {
                int pivot = -1;
                for (int j = i; j < row; j++)
                {
                    if (a[j][i].value() != 0)
                    {
                        pivot = j;
                        break;
                    }
                }
                if (pivot == -1) throw std::invalid_argument("Singular matrix has no inverse.");
                if (pivot != i)
                {
                    swap(a[pivot], a[i]);
                    swap(res[pivot], res[i]);
                }
                ModInt inv = a[i][i].inv();
                for (int j = 0; j < row; j++)
                {
                    a[i][j] *= inv;
                    res[i][j] *= inv;
                }
                for (int j = 0; j < row; j++)
                {
                    if (j == i) continue;
                    ModInt ratio = a[j][i];
                    for (int k = 0; k < row; k++)
                    {
                        a[j][k] -= ratio * a[i][k];
                        res[j][k] -= ratio * res[i][k];
                    }
                }
            }
            return res;
        }
    };

    inline ModMatrix operator*(const ModMatrix& a, const ModMatrix& b)
    {
        if (a.col != b.row) throw std::invalid_argument("Two matrices are not able to product.");
        if (a.MOD != b.MOD) throw std::invalid_argument("Two matrices have different MOD.");
        ModInt::SetMod(a.MOD);
        ModMatrix res(a.row, b.col, a.MOD);
        for (int i = 0; i < a.row; i++)
            for (int j = 0; j < b.col; j++)
                for (int k = 0; k < a.col; k++)
                    res[i][j] += a[i][k] * b[k][j];
        return res;
    }

    inline ModMatrix GetIdenticalModMatrix(int n, long long MOD = ModInt::MOD)
    {
        ModMatrix res(n, n, MOD);
        for (int i = 0; i < n; i++) res[i][i] = 1;
        return res;
    }

    inline ModInt Det(const ModMatrix& a) { return a.determinant(); }
    inline ModMatrix Inverse(const ModMatrix& a) { return a.inverse(); }
};
