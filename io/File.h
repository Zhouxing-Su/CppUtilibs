////////////////////////////////
/// usage : 1.	file operations.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_FILE_H
#define SZX_CPPUTILIBS_FILE_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


namespace szx {

class File {
public:
    static std::string readAll(std::ifstream &fin) {
        std::ostringstream oss;
        oss << fin.rdbuf();
        return oss.str();
    }
    static std::string readAll(const std::string &path) {
        std::ifstream ifs(path);
        if (!ifs.is_open()) { return ""; }
        std::ostringstream oss;
        oss << ifs.rdbuf();
        return oss.str();
    }
};

}


#endif // SZX_CPPUTILIBS_FILE_H