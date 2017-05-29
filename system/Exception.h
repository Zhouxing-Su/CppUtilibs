////////////////////////////////
/// usage : 1.	some useful exceptions.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_EXCEPTION_H
#define SZX_CPPUTILIBS_EXCEPTION_H


#include <exception>


namespace szx {

struct NotImplementedException : public std::exception {
    virtual const char* what() const override {
        return "not implemented yet.";
    }
};

struct PeekEmptyContainerException : public std::exception {
    virtual const char* what() const override {
        return "working with an empty container.";
    }
};

}


#endif // SZX_CPPUTILIBS_EXCEPTION_H
