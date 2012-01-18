#include "utilities.hpp"

std::string dumpSStream( std::stringstream& stream )
{
    std::string ret = stream.str();
    stream.str( std::string() );
    return ret;
}



void divmod( int dividend, int divisor, int &quotient, int &remainder )
{
    quotient = dividend / divisor;
    remainder = dividend - divisor * quotient;
}



void meanStdvPrsd( const std::vector<double>& vals, double& mean, double& stdv, double& prsd, bool nm1d=true )
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
