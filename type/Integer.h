/**
*   usage : 1.  define a big integer which is harder to be overflow than int.
*
*   note :  1.  should constructor be explicit ?
*/

#ifndef INTEGER_H
#define INTEGER_H


#include <iostream>
#include <string>
#include <vector>


namespace szx
{
    class Integer
    {
    public:
        typedef int Digit;


        static const int RADIX;


        Integer( int i = 0 );
        Integer( const std::string &s );

        // meta infomation
        int getDigitNum() const;

        // assignments operator
        Integer& operator+=(const Integer& n);
        Integer& operator-=(const Integer& n);
        Integer& operator*=(const Integer& n);
        Integer& operator/=(const Integer& n);
        Integer& operator%=(const Integer &n);

        Integer& operator++();
        Integer& operator--();
        Integer& operator++(int);
        Integer& operator--(int);

        // comparison operator
        bool operator==(const Integer &n);
        bool operator!=(const Integer &n);
        bool operator<(const Integer &n);
        bool operator>(const Integer &n);
        bool operator<=(const Integer &n);
        bool operator>=(const Integer &n);

        // arithmetic operator
        friend Integer operator+(const Integer &l, const Integer &r);
        friend Integer operator-(const Integer &l, const Integer &r);
        friend Integer operator*(const Integer &l, const Integer &r);
        friend Integer operator/(const Integer &l, const Integer &r);
        friend Integer operator%(const Integer &l, const Integer &r);

        // I/O and convertion
        friend std::istream& operator>>(std::istream &is, const Integer &i);
        friend std::ostream& operator<<(std::ostream &os, const Integer &i);

    private:
        std::vector<Digit> digits;
    };
}


#endif