////////////////////////////////
/// usage : 1.	algorithms for string processing.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_STRING_H
#define SZX_CPPUTILIBS_STRING_H


#include <algorithm>
#include <string>

#include "../type/Arr.h"


namespace szx {

class String {
public:
    static void traceEdit(const Arr2D<int> &dist) {
        traceEdit(dist, dist.size1() - 1, dist.size2() - 1);
        std::cout << std::endl;
    }
    static void traceEdit(const Arr2D<int> &dist, int i, int j) {
        if ((i == 0) && (j == 0)) { return; }

        if ((i == 0) || (dist.at(i - 1, j) + 1 == dist.at(i, j))) {
            traceEdit(dist, i - 1, j);
            std::cout << 'D';
        } else if ((j == 0) || (dist.at(i, j - 1) + 1 == dist.at(i, j))) {
            traceEdit(dist, i, j - 1);
            std::cout << 'I';
        } else {
            traceEdit(dist, i - 1, j - 1);
            if (dist.at(i - 1, j - 1) == dist.at(i, j)) {
                std::cout << 'K';
            } else { // if (dist.at(i - 1, j - 1) + 1 == dist.at(i, j)) {
                std::cout << 'C';
            }
        }
    }

    static int shortestEditDistance(const std::string &src, const std::string &dst) {
        const char *x = src.data() - 1;
        const char *y = dst.data() - 1;
        // dist[i, j] == shortest edit distance from x[0..i] to y[0..j].
        Arr2D<int> dist(src.size() + 1, dst.size() + 1);
        for (int i = 0; i < dist.size1(); ++i) { dist.at(i, 0) = i; }
        for (int i = 1; i < dist.size2(); ++i) { dist.at(0, i) = i; }
        for (int i = 1; i < dist.size1(); ++i) {
            for (int j = 1; j < dist.size2(); ++j) {
                dist.at(i, j) = (std::min)(
                    (dist.at(i - 1, j - 1) + (x[i] != y[j])),
                    ((std::min)(dist.at(i - 1, j), dist.at(i, j - 1)) + 1));
            }
        }

        traceEdit(dist);

        return dist.back();
    }
};

}


#endif // SZX_CPPUTILIBS_STRING_H
