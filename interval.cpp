#include "interval.h"

using namespace std;

Interval::Interval()
    : _lo(0), _hi(0), _loOpen(true), _hiOpen(true){}



Interval::Interval( double lo, double hi )
    : _lo(lo), _hi(hi), _loOpen(true), _hiOpen(true){}



Interval::Interval( double lo, double hi, bool loOpen, bool hiOpen )
    : _lo(lo), _hi(hi), _loOpen(loOpen), _hiOpen(hiOpen){}



double Interval::lo() const
{
    return _lo;
}



double Interval::hi() const
{
    return _hi;
}



double Interval::mid() const
{
    return _lo + ( _hi - _lo ) / 2.0;
}



double Interval::dist() const
{
    return _hi - _lo;
}


bool Interval::in( double val )
{
    return ( _loOpen ? val > _lo : val >= _lo ) && ( _hiOpen ? val < _hi : val <= _hi );
}



double Interval::rand()
{
    return R.randd( _lo, _hi );
}



double Interval::randg( double sigmaSF )
{
    double mu = ( _hi - _lo ) / 2.0;
    double sigma = _hi - _lo / ( sigmaSF * 2.0 );
    double r = 0;
    do { r = R.randg( mu, sigma ); } while( !in(r) );
    return r;
}



double Interval::clamp( double val )
{
    double clampedVal = val;
    if( val <= _lo )
        clampedVal = _lo + _loOpen ? DBL_EPSILON : 0.0;
    else if( val >= _hi )
        clampedVal = _hi - _hiOpen ? DBL_EPSILON : 0.0;
    return clampedVal;
}



Interval Interval::operator+( double offset )
{
    return Interval( _lo + offset, _hi + offset, _loOpen, _hiOpen );
}



Interval Interval::operator-( double offset )
{
    return *this + -offset;
}



void Interval::shift( double offset )
{
    _lo += offset;
    _hi += offset;
}



std::vector<double> Interval::subdividers( int n )
{
    std::vector<double> divs;
    double dv = dist() / n;
    for( double v=_lo; v<=_hi; v+=dv )
        divs.push_back( v );
    return divs;
}



std::vector<double> Interval::subdividers( double dv=1.0 )
{
    std::vector<double> divs;
    for( double v=_lo; v<=_hi; v+=dv )
        divs.push_back( v );
    return divs;
}

//think about how the subintervals would be open or closed
//    std::vector<Interval> subdivide( int n )
//    {
//
//    }



string Interval::str()
{
    return string( (_loOpen?"( ":"[ ") + num2str(_lo) + ", " + num2str(_hi) + (_hiOpen?" )":" ]") );
}

ostream& operator<<( ostream& out, Interval& i )
{
    return out << i.str();
}
