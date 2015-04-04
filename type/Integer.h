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

        enum Sign { NEGATIVE = -1, ZERO = 0, POSITIVE = 1 };

        static const int DigitLen = 4;
        static const int RADIX = 10000;


        // meta information
        static Sign getSign( int i );
        static Sign getSign( const std::string &s );

        Integer( int i = 0 );
        Integer( const std::string &s );

        Sign getSign() const;
        int getDigitNum() const;

        // assignments operator
        Integer& operator+=(const Integer& n);
        Integer& operator-=(const Integer& n);
        Integer& operator*=(const Integer& n);
        Integer& operator/=(const Integer& n);
        Integer& operator%=(const Integer &n);

        Integer& operator++();
        Integer& operator--();
        Integer operator++(int);
        Integer operator--(int);

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

        // I/O and conversion
        friend std::istream& operator>>(std::istream &is, const Integer &i);
        friend std::ostream& operator<<(std::ostream &os, const Integer &i);

    private:
        Sign sign;
        std::vector<Digit> digits;
    };
}


#endif