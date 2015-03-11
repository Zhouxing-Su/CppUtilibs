#include "Integer.h"


using namespace std;


namespace szx
{
    const int Integer::RADIX = 10000;


    Integer::Integer( int i )
    {

    }

    Integer::Integer( const std::string &s )
    {

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