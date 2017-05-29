////////////////////////////////
/// usage : 1.	portable simplify directory operation.
/// 
/// note  : 1.  not efficient and secure.
///         2.  add space on the border of commands.
////////////////////////////////

#ifndef SZX_CPPUTILIBS_DIRECTORY_H
#define SZX_CPPUTILIBS_DIRECTORY_H


#include <string>


namespace szx {

class Directory {
public:
    static void makeSureDirExist(const std::string &dir);

    static std::string quote(const std::string &s) { return ('\"' + s + '\"'); }
};

}


#endif // SZX_CPPUTILIBS_DIRECTORY_H