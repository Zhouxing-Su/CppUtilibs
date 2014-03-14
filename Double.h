/**
*   usage : [deprecated] wrap the double to overcome the problems in comparison due to precision loss.
*           you can set the error by accessing the static member 'error'.
*/

#ifndef DOUBLE_H
#define DOUBLE_H

#include <cmath>

class Double
{
    double value;
public:
    static double error;

    Double() : value() {}
    Double( double d ) : value( d ) {}
    operator double() const { return value; }

    // assignments
    Double& operator=(double d) { value = d; return *this; }
    Double& operator+=(double d) { value += d; return *this; }
    Double& operator-=(double d) { value -= d; return *this; }
    Double& operator*=(double d) { value *= d; return *this; }
    Double& operator/=(double d) { value /= d; return *this; }
    Double& operator++() { ++value; return *this; }
    Double& operator--() { --value; return *this; }
    Double operator++(int) { return Double( value++ ); }
    Double operator--(int) { return Double( value-- ); }

    // comparison
    friend bool operator==(const Double &D1, const Double &D2) { return (fabs( D1.value - D2.value ) <= error); }
    friend bool operator==(const Double &D, const double &d) { return (fabs( D.value - d ) <= error); }
    friend bool operator==(const double &d, const Double &D) { return (fabs( d - D.value ) <= error); }
    friend bool operator!=(const Double &D1, const Double &D2) { return (fabs( D1.value - D2.value ) > error); }
    friend bool operator!=(const Double &D, const double &d) { return (fabs( D.value - d ) > error); }
    friend bool operator!=(const double &d, const Double &D) { return (fabs( d - D.value ) > error); }
    friend bool operator<(const Double &D1, const Double &D2) { return ((D1.value - D2.value) < -error); }
    friend bool operator<(const Double &D, const double &d) { return ((D.value - d) < -error); }
    friend bool operator<(const double &d, const Double &D) { return ((d - D.value) < -error); }
    friend bool operator>(const Double &D1, const Double &D2) { return ((D1.value - D2.value) > error); }
    friend bool operator>(const Double &D, const double &d) { return ((D.value - d) > error); }
    friend bool operator>(const double &d, const Double &D) { return ((d - D.value) > error); }
    friend bool operator<=(const Double &D1, const Double &D2) { return ((D1.value - D2.value) <= error); }
    friend bool operator<=(const Double &D, const double &d) { return ((D.value - d) <= error); }
    friend bool operator<=(const double &d, const Double &D) { return ((d - D.value) <= error); }
    friend bool operator>=(const Double &D1, const Double &D2) { return ((D1.value - D2.value) >= -error); }
    friend bool operator>=(const Double &D, const double &d) { return ((D.value - d) >= -error); }
    friend bool operator>=(const double &d, const Double &D) { return ((d - D.value) >= -error); }

    // other binary operators will work by converting Double to double implicitly.
};


#endif