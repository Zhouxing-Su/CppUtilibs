////////////////////////////////
/// usage : 1.	basic arithmetic functions.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_MATH_H
#define SZX_CPPUTILIBS_MATH_H


#include <cmath>

#include "../system/System.h"


#if _CC_MSVC
#include <intrin.h> // for `log2()`.
#elif _CC_GNU_GCC
#include <climits> // for `log2()`.
#endif // _CC_MSVC


namespace szx {

class Math {
public:
    class Impl {
    public:
        using Int = int32_t;
        using Uint = uint32_t;
        using Long = long;
        using Ulong = unsigned long;

        #pragma region log2
        static Uint log2v0(Uint n) {
            return static_cast<Uint>(std::log2(n));
        }

        #if _CC_MSVC
        // http://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c
        static Ulong log2v1(Ulong n) {
            Ulong r;
            _BitScanReverse(&r, n);
            return r;
        }
        #elif _CC_GNU_GCC
        // http://stackoverflow.com/questions/994593/how-to-do-an-integer-log2-in-c/22418446#22418446
        static Uint log2v1(Uint x) {
            return static_cast<Uint>(((sizeof(Int) * CHAR_BIT) - 1) - __builtin_clz(x));
        }
        #endif // _CC_MSVC

        // http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup
        static Uint log2v2(Uint n) {
            #define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
            static const char LogTable256[256] = {
                -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
                LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
            };
            #undef LT

            Uint r;
            Uint tt = (n >> 16);
            if (tt) {
                Uint t = (tt >> 8);
                r = (t ? (24 + LogTable256[t]) : (16 + LogTable256[tt]));
            } else {
                Uint t = (n >> 8);
                r = (t ? (8 + LogTable256[t]) : LogTable256[n]);
            }
            return r;
        }

        // http://guihaire.com/code/?p=414
        static Uint log2v3(Uint n) {
            union {
                float f;
                Uint n;
            } u;
            u.f = static_cast<float>(n);
            return ((u.n >> 23) - 127);
        }

        // http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
        static Uint log2v4(Uint n) {
            static const Uint MultiplyDeBruijnBitPosition[32] = {
                0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
                8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
            };

            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;

            return MultiplyDeBruijnBitPosition[static_cast<Uint>(n * 0x07C4ACDDU) >> 27];
        }

        // http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog
        static Uint log2v5(Uint n) {
            const Uint b[] = { 0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
            const Uint S[] = { 1, 2, 4, 8, 16 };

            Uint r = 0;
            for (Int i = 4; i >= 0; --i) {
                if (n & b[i]) {
                    n >>= S[i];
                    r |= S[i];
                }
            }
            return r;
        }

        // http://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c/9411984#9411984
        static Uint log2v6(Uint n) {
            int r;
            std::frexp(n, &r);
            return static_cast<Uint>(r - 1);
        }
        #pragma endregion log2
    };


    static int round(double num) { return static_cast<int>(num + 0.5); }

    template<typename T>
    static T power2(T num) { return (num * num); }

    static unsigned log2(unsigned n) {
        #if _CC_MSVC
        return Impl::log2v1(n);
        #elif _CC_GNU_GCC
        return Impl::log2v3(n);
        #else
        return Impl::log2v2(n);
        #endif // _CC_MSVC
    }

    template<typename T>
    static bool updateMin(T &minItem, const T &newItem) {
        if (newItem >= minItem) { return false; }
        minItem = newItem;
        return true;
    }

    template<typename T>
    static bool updateMax(T &maxItem, const T &newItem) {
        if (newItem <= maxItem) { return false; }
        maxItem = newItem;
        return true;
    }

    template<typename T>
    static T average(T num1, T weight1, T num2, T weight2) {
        return ((num1 * weight1) + (num2 * weight2)) / (weight1 + weight2);
    }

    // normalize the list to [0, maxValue].
    template<typename T, typename List>
    static void normalize(List &list, T maxValue, T minItem, T maxItem) {
        T range = maxItem - minItem;
        for (auto iter = list.begin(); iter != list.end(); ++iter) {
            ((*iter -= minItem) *= maxValue) /= range;
        }
    }
    template<typename T, typename List>
    static void normalize(List &list, T maxValue) {
        auto itemRange = std::minmax_element(list.begin(), list.end());
        normalize(list, maxValue, *(itemRange.first), *(itemRange.second));
    }
};

}


#endif // SZX_CPPUTILIBS_MATH_H
