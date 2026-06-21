#pragma once
#include <bits/stdc++.h>

using namespace std;

namespace zty
{
    template <typename T, typename = void>
    class has_debug_method : public false_type {};

    template <typename T>
    class has_debug_method<T, void_t<decltype(declval<const T&>().debug(declval<ostream&>()))>> : public true_type {};

    template <typename T, typename = void>
    class can_output_stream : public false_type {};

    template <typename T>
    class can_output_stream<T, void_t<decltype(declval<ostream&>() << declval<const T&>())>> : public true_type {};

    /*
        To make a new class compatible with Debugger, define this member:

        #ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "StructName{ important_field=" << important_field << " }\n";
        }
        #endif

        Then call:
            Debugger dbg;
            dbg.log("name", object);
    */
    class Debugger
    {
    public:
#ifdef ZTY_DEBUG
        ostream* out;

        Debugger()
        {
#ifdef ZTY_DEBUG_STDOUT
            out = &cout;
#else
            out = &cerr;
#endif
        }
        Debugger(ostream& os) { out = &os; }

        template <typename T>
        void dump(const T& x) const
        {
            if constexpr (has_debug_method<T>::value) x.debug(*out);
            else if constexpr (can_output_stream<T>::value) (*out) << x << '\n';
            else (*out) << "[No debug function or operator<< available]\n";
        }

        template <typename T>
        void log(const string& name, const T& x) const
        {
            (*out) << "[Debug] " << name << ":\n";
            dump(x);
        }

        template <typename T>
        void operator()(const string& name, const T& x) const
        {
            log(name, x);
        }
#else
        Debugger() {}
        Debugger(ostream&) {}

        template <typename T>
        void dump(const T&) const {}

        template <typename T>
        void log(const string&, const T&) const {}

        template <typename T>
        void operator()(const string&, const T&) const {}
#endif
    };
}
