#pragma once

#include <vector>
#include <string>
#include <ostream>

#include "randmaster.h"

/** Utility class used to represent a numeric interval.  The intervals can be open or closed on either end independently
  * and various tests and numbers can be generated within this interval
  * @todo  Perhaps template this class so that it can be used with integers and other numeric types
  */
class Interval
{
protected:

    /// The low end of the interval
    double _lo;

    /// The high end of the interval
    double _hi;

    /// Specifies whether the low end is open
    bool _loOpen;

    /// Specifies whether the high end is open
    bool _hiOpen;

    /// A random number generator for producing random values from this interval
    RandMaster R;



public:

    /// Constructs a fully open interval on the range ( 0, 1 )
    Interval();

    /// Constructs a fully open interval on the range ( lo, hi )
    Interval(
            double lo, ///< The low end of the interval
            double hi  ///< The hi end of the interval
            );

    /// Constructs an interval with specified openness
    Interval(
            double lo,   ///< The low end of the interval
            double hi,   ///< The high end of the interval
            bool loOpen, ///< Specifies if the low end of the interval is open or closed
            bool hiOpen  ///< Specifies if the high end of the interval is open or closed
            );

    /// Fetches the value of the low end of the interval
    double lo() const;

    /// Fetches the value of the high end of the interval
    double hi() const;

    /// Fetches the midpoint of the interval
    double mid() const;

    /// Fetches the distance between the high and low end of the interval
    double dist() const;

    /// Determines if a given value lies within the interval
    bool in( double val );

    /// Generates a random value with uniform distribution from within the interval
    double rand();

    /// Generates a random value with gaussian distribution centered on the midpoint of the interval
    double randg(
            double sigmaSF=3.0 ///< Specifies how many standard deviations span the lower half of the interval
            );

    /// Clamps a value within the interval
    double clamp( double val );

    /// Creates a copy of this interval that has been moved by the specified offset
    Interval operator+( double offset );

    /// Creates a copy of this interval that has been moved by the opposite of the specified offset
    Interval operator-( double offset );

    /// Moves this interval by the specified offset
    void shift( double offset );

    /// Generates a list of n evenly spaced subdividing values for this interval
    std::vector<double> subdividers( int n );

    /// Generates a list of subdividers for the interval that are seperated by the given distance
    std::vector<double> subdividers( double dv=1.0 );

    /// Returns a string representation of the interval
    std::string str();

    /// Overloads the ostream operator for printing
    friend std::ostream& operator<<( std::ostream& out, Interval& i );
};
