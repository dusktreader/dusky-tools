#pragma once

#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "tools/local_assert/local_assert.h"
#include "math_utilities.hpp"

/** Converts a number to a string
  * @param  number - The number to convert
  * @param  prec   - Floating point precision to use
  * @param  w      - Width of the string
  * @return A string representation of the number
  */
template <class T>
std::string num2str( T number, int prec=2, int w=0 )
{
    std::ostringstream ss;
    ss.setf( std::ios::showpoint | std::ios::fixed );
    ss.precision( prec );
    ss.fill( ' ' );
    ss.width( w );
    ss << number;
    return ss.str();
}

/** Converts a string to a number
  * @param  str    - The string to convert
  * @param  number - The storage for the number
  */
template <class T>
void str2num( std::string str, T &number )
{
    std::istringstream ss( str );
    ss >> number;
}

template <typename R, typename I>
R convert( const I& in )
{
    std::stringstream ss;
    ss << in;
    R ret;
    ASSERT_MSG( !( ss >> ret ).fail(), "Conversion failed" );
    return ret;
}

/// Converts a value with units in degrees to units of radians
double toRadians( double degrees );

/// Converts a value with units in radians to units of degrees
double toDegrees( double radians );






/// Converts a character to an integer
int char2Int( const char a );

/** @brief   Converts a character to an integer index
  * @example char2Idx( 'a' ) == 0
  * @example char2Idx( 'D' ) == 3
  */
int char2Idx( char c );

/** @brief   Converts an integer to a character index
  * @example idx2Char( 0, true  ) == 'a'
  * @example idx2Char( 3, false ) == 'D'
  */
char idx2Char( int i, bool lowerCase=false );

///  Converts a linear array index into a set of coordiantes for an n-dimensional cartesian space
void index2coords(
        int idx,                      ///< The linear array index to convert
        const std::vector<int> &dims, ///< The dimension sizes of the cartesian n-space
        std::vector<int> &coords      ///< The cartesian coordinates to convert
        );

/// Converts a set of coordinates for an n-dimensional cartesian space into a linear array index
void coords2index(
        int &idx,                      ///< The store reference for the calculated index
        const std::vector<int> &dims,  ///< The dimension sizes of the cartesian n-space
        const std::vector<int> &coords ///< The cartesian coordinates to convert
        );
