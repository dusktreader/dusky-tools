#include "tools.h"


/* ----------------- */

int char2Int( char a ){
    int i;
    ASSERT_SET( i, (int)(a-'0'), a >= '0' && a <= '9', "Character must be a numeral" );
    return i;
}

int char2Idx( char c )
{
    ASSERT_MSG( ( c >= 'a' && c <= 'z' ) ||
                ( c >= 'A' && c <= 'Z' ),
                "Character must be in the standard alphabet" );
    char baseChar = c >= 'a' && c <= 'z' ? 'a' : 'A';
    return c - baseChar;
}

char idx2Char( int i, bool lowerCase )
{
    ASSERT_MSG( i >=0 && i < 26, "Index exceeds range of standard alphabet" );
    char baseChar = lowerCase ? 'a' : 'A';
    return baseChar + i;
}

void divmod( int dividend, int divisor, int &quotient, int &remainder )
{
    quotient = dividend / divisor;
    remainder = dividend - divisor * quotient;
}

void coords2index( int &idx, const vector<int> &dims, const vector<int> &coords )
{
    ASSERT_MSG( coords.size() == dims.size(), "Dimension missmatch" );
    for( int i=0; i<(int)coords.size(); i++ )
        ASSERT_MSG( coords[i] < dims[i], "Coorinate " + num2str(i) + " exceeds dimensional limit" );
    idx = 0;
    int t;
    for( int i=0; i<(int)dims.size(); i++ ){
        t = coords[i];
        for( int j=i+1; j<(int)dims.size(); j++ )
            t *= dims[j];
        idx += t;
    }
}

string dumpSStream( stringstream& stream )
{
    string ret = stream.str();
    stream.str( std::string() );
    return ret;
}

void index2coords( int idx, const vector<int> &dims, vector<int> &coords ){
    int t = idx;
    coords = vector<int>( dims.size(), 0 );
    for( int i=dims.size()-1; i>1; i-- )
        divmod( t, dims[i], t, coords[i] );
    divmod( t, dims[1], coords[0], coords[1] );
}



void meanStdvPrsd( const vector<double>& vals, double& mean, double& stdv, double& prsd, bool nm1d )
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
