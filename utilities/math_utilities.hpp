#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cfloat>

#include "convert_utilities.hpp"

#ifndef PI
#define PI 3.1415926535897931
#endif

#ifndef EPS
#define EPS 1e-10
#endif


/// Squares a numerical value
template <class T>
inline T pow2( const T& val )
{
    return val * val;
}

/// @brief  Performs a division and modulo operation
void divmod(
        int dividend,  ///< The numerator of the division
        int divisor,   ///< The denominator of the division
        int &quotient, ///< The result of the division
        int &remainder ///< The result for modulo of the division
        );



/** Calculates the Mean, Standard Deviation, and %RSD of a given group of
 *  floating point values stored in a random access container
 */
template <class T>
void meanStdvPrsd_RA(
        const T& vals, ///< The values for which to compute the statistics
        double& mean,  ///< A reference to the final mean value
        double& stdv,  ///< A reference to the final standard deviation value
        double& prsd,  ///< A reference to the final percent RSD value
        bool nm1d=true ///< Use an (n-1) divisor for the standard deviation
        )
{
    int n = vals.size();
    mean = 0.0;
    for( int i=0; i<n; i++ )
        mean += vals[i];
    mean /= n;

    stdv = 0.0;
    for( int i=0; i<n; i++ )
        stdv += pow( vals[i] - mean, 2.0 );
    stdv /= nm1d ? n - 1 : n;
    stdv = pow( stdv, 0.5 );

    prsd = stdv / mean * 100.0;
}



/** Calculates the Mean, Standard Deviation, and %RSD of a given group of
 *  floating point values stored in a sequential access container
 */
template <class T>
void meanStdvPrsd_SA(
        const T& vals, ///< The values for which to compute the statistics
        double& mean,  ///< A reference to the final mean value
        double& stdv,  ///< A reference to the final standard deviation value
        double& prsd,  ///< A reference to the final percent RSD value
        bool nm1d=true ///< Use an (n-1) divisor for the standard deviation
        )
{
    int n = vals.size();
    mean = 0.0;
    for( typename T::const_iterator itr = vals.begin(); itr != vals.end(); itr++ )
        mean += *itr;
    mean /= n;

    stdv = 0.0;
    for( typename T::const_iterator itr = vals.begin(); itr != vals.end(); itr++ )
        stdv += pow( *itr - mean, 2.0 );
    stdv /= nm1d ? n - 1 : n;
    stdv = pow( stdv, 0.5 );

    prsd = stdv / mean * 100.0;
}


/** Convenience class used for generating random integers, and reals with uniform and gaussian distributions within
  * ranges defined when the functions are called
  *
  * @todo:  Add more distributions
  */
class RandMaster
{
private:

    /// The last random gaussian number to be generated
    double lastGauss;

    /// The seed value for the random generator.  Shared by all RandMasters in the current application
    static unsigned int seed;

public:

    /// Constructs a RandMaster object
    RandMaster();

    /// Generates a random integer from within a uniform distribution in the specified range
    int randi(
            int lo=0,  ///< The low end of the range
            int hi=100 ///< The high end of the range
            );

    /// Returns a random real value from with a uniform distribution in the specified range
    float randf(
            float lo=0.0, ///< The low end of the range
            float hi=1.0  ///< The high end of the range
            );

    /// Returns a random double with a uniform distribution in the specified range
    double randd(
            double lo=0.0, ///< The low end of the range
            double hi=1.0  ///< The high end of the range
            );

    /** Generates a random real value from within a gaussian distribution
      * @note   This algorithm uses the Box-Mueller transformation method described in Numerical Recipes 3rd Ed p364
      */
    double randg(
            double mu=0.0,   ///< The center of the distribution
            double sigma=1.0 ///< The standard deviation of the distribution
            );

    /// Generates a boolean value within a given probability of a 'true' or '1' outcome
    bool randp(
            double p ///< The probablility of an 'true' or '1' outcome
            );
};



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
