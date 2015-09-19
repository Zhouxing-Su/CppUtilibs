#include "Integer.h"


using namespace std;


namespace szx
{

Integer::Sign Integer::getSign( int i )
{
    if (i > 0) {
        return Sign::POSITIVE;
    } else if (i < 0) {
        return Sign::NEGATIVE;
    } else {
        return Sign::ZERO;
    }
}

Integer::Sign Integer::getSign( const std::string &s )
{
    if (s[0] == '-') {
        return Sign::NEGATIVE;
    } else if (s[0] == '0') {
        return Sign::ZERO;
    } else {
        return Sign::POSITIVE;
    }
}

Integer::Integer( int i )
    : sign( getSign( i ) )
{
    i *= sign;

    while (i != 0) {
        digits.push_back( i % RADIX );
        i /= RADIX;
    }
}

Integer::Integer( const std::string &s )
    : sign( getSign( s ) )
{
    for (string::const_reverse_iterator iter = s.rbegin();
        iter != s.rend(); iter++) {
        // TODO:

    }
}

Integer::Sign Integer::getSign() const
{
    return sign;
}

int Integer::getDigitNum() const
{
    // TODO: the most significant digit may be 0
    return digits.size() * DigitLen;
}

Integer& Integer::operator+=(const Integer& n)
{

    return *this;
}

Integer& Integer::operator-=(const Integer& n)
{

    return *this;
}

Integer& Integer::operator*=(const Integer& n)
{

    return *this;
}

Integer& Integer::operator/=(const Integer& n)
{

    return *this;
}

Integer& Integer::operator%=(const Integer &n)
{

    return *this;
}

Integer& Integer::operator++()
{

    return *this;
}

Integer Integer::operator++(int)
{
    Integer i( *this );
    ++(*this);
    return i;
}

Integer& Integer::operator--()
{

    return *this;
}

Integer Integer::operator--(int)
{
    Integer i( *this );
    --(*this);
    return i;
}

bool Integer::operator==(const Integer &n)
{

    return true;
}

bool Integer::operator!=(const Integer &n)
{

    return true;
}

bool Integer::operator<(const Integer &n)
{

    return true;
}

bool Integer::operator>(const Integer &n)
{

    return true;
}

bool Integer::operator<=(const Integer &n)
{

    return true;
}

bool Integer::operator>=(const Integer &n)
{

    return true;
}

Integer operator+(const Integer &l, const Integer &r)
{
    Integer i( l );
    return  (i += r);
}

Integer operator-(const Integer &l, const Integer &r)
{
    Integer i( l );
    return  (i -= r);
}

Integer operator*(const Integer &l, const Integer &r)
{
    Integer i( l );
    return  (i *= r);
}

Integer operator/(const Integer &l, const Integer &r)
{
    Integer i( l );
    return  (i /= r);
}

Integer operator%(const Integer &l, const Integer &r)
{
    Integer i( l );
    return  (i %= r);
}

istream& operator>>(std::istream &is, const Integer &i)
{

    return is;
}

ostream& operator<<(std::ostream &os, const Integer &i)
{

    return os;
}

}