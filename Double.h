/**
*   usage : wrap the double to overcome the problems in comparison due to precision loss.
*           you can set the error by accessing the static member 'error'.
*/

#ifndef DOUBLE_H

#include <cmath>

class Double
{
    double value;
public:
    static double error;

    Double() : value() {}
    Double( double d ) : value( d ) {}
    operator double() const { return value; }

    //assignments
    Double& operator=(double d) { value = d; return *this; }
    Double& operator+=(double d) { value += d; return *this; }
    Double& operator-=(double d) { value -= d; return *this; }
    Double& operator*=(double d) { value *= value; return *this; }
    Double& operator/=(double d) { value /= value; return *this; }
    Double& operator++() { ++value; return *this; }
    Double& operator--() { --value; return *this; }
    Double operator++(int) { return Double( value++ ); }
    Double operator--(int) { return Double( value-- ); }
    //accessors
    Double operator+() const { return Double( +value ); }
    Double operator-() const { return Double( -value ); }
    Double operator!() const { return Double( !value ); }
    double* operator&() { return &value; }
    //binary operators
    friend Double operator+(Double D1, Double D2) { return D1 += D2; }
    friend Double operator+(Double D, double d) { return D += d; }
    friend Double operator+(double d, Double D) { return Double( d ) += D; }
    friend Double operator-(Double D1, Double D2) { return D1 -= D2; }
    friend Double operator-(Double D, double d) { return D -= d; }
    friend Double operator-(double d, Double D) { return Double( d ) -= D; }
    friend Double operator*(Double D1, Double D2) { return D1 *= D2; }
    friend Double operator*(Double D, double d) { return D *= d; }
    friend Double operator*(double d, Double D) { return Double( d ) *= D; }
    friend Double operator/(Double D1, Double D2) { return D1 /= D2; }
    friend Double operator/(Double D, double d) { return D /= d; }
    friend Double operator/(double d, Double D) { return Double( d ) /= D; }

    friend bool operator==(const Double &D1, const Double &D2) { return (fabs( D1.value - D2.value ) < error); }
    friend bool operator==(const Double &D, const double &d) { return (fabs( D.value - d ) < error); }
    friend bool operator==(const double &d, const Double &D) { return (fabs( d - D.value ) < error); }
    friend bool operator<(const Double &D1, const Double &D2) { return ((D1.value - D2.value) < -error); }
    friend bool operator!=(const Double &D1, const Double &D2) { return (fabs( D1.value - D2.value ) > error); }
    friend bool operator!=(const Double &D, const double &d) { return (fabs( D.value - d ) > error); }
    friend bool operator!=(const double &d, const Double &D) { return (fabs( d - D.value ) > error); }
    friend bool operator<(const Double &D, const double &d) { return ((D.value - d) < -error); }
    friend bool operator<(const double &d, const Double &D) { return ((d - D.value) < -error); }
    friend bool operator>(const Double &D1, const Double &D2) { return ((D1.value - D2.value) > error); }
    friend bool operator>(const Double &D, const double &d) { return ((D.value - d) > error); }
    friend bool operator>(const double &d, const Double &D) { return ((d - D.value) > error); }
    friend bool operator<=(const Double &D1, const Double &D2) { return ((D1.value - D2.value) < error); }
    friend bool operator<=(const Double &D, const double &d) { return ((D.value - d) < error); }
    friend bool operator<=(const double &d, const Double &D) { return ((d - D.value) < error); }
    friend bool operator>=(const Double &D1, const Double &D2) { return ((D1.value - D2.value) > -error); }
    friend bool operator>=(const Double &D, const double &d) { return ((D.value - d) > -error); }
    friend bool operator>=(const double &d, const Double &D) { return ((d - D.value) > -error); }
};


#define DOUBLE_H
#endif