#include "tools.h"

using namespace std;

/* Methods for the LocalAssert Class */
LocalAssert::Status LocalAssert::status = LocalAssert::NoError;

LocalAssert::LocalAssert( string info, string message )
    : _message(message), _info(info)
{
    /*
    void* btBuff[1000];
    int btSize;
    char** btStrings;

    btSize = backtrace( btBuff, 1000 );
    btStrings = backtrace_symbols( btBuff, btSize );

    _trace = "";
    for( int i = 0; i < btSize; i++ )
        _trace.append( btStrings[i] ).append( "\n" );

    free( btStrings );
    */


    status = Error;
}

LocalAssert::~LocalAssert() throw()
{}

const char* LocalAssert::what() const throw()
{
    return _message.c_str();
}

string LocalAssert::message()
{
    return _message;
}

string LocalAssert::where()
{
    return _info;
}

string LocalAssert::trace()
{
    return _trace;
}

string LocalAssert::details()
{
    string msg;
    msg += "============\n";
    msg += "LocalAssert:\n";
    msg += "------------\n";
    msg += message() + "\n";
    msg += "------------\n";
    msg += where() + "\n";
    msg += "------------\n";
    msg += trace() + "\n";
    msg += "============\n";
    return msg;
}

LocalAssert::Status LocalAssert::fetchStatus()
{
    Status s = status;
    status = NoError;
    return s;
}

void LocalAssert::resetStatus()
{
    status = NoError;
}

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

unsigned int RandMaster::seed = 0;

RandMaster::RandMaster()
{
    if( seed == 0 )
        seed = time(0);
    else
        seed += 7;
    srand( seed );
    lastGauss = DBL_MAX;
}

int RandMaster::randi( int lo, int hi )
{
    return lo + rand() % ( hi - lo );
}

float RandMaster::randf( float lo, float hi )
{
    return lo + ( (float)rand() / RAND_MAX )  * ( hi - lo );
}

double RandMaster::randd( double lo, double hi )
{
    return lo + ( (double)rand() / RAND_MAX ) * ( hi - lo );
}

double RandMaster::randg( double mu, double sigma )
{
    double v1  = 0.0,
           v2  = 0.0,
           rsq = 0.0,
           fac = 0.0;
    if( lastGauss == DBL_MAX )
    {
        while( rsq >= 1.0 || rsq == 0.0 )
        {
            v1 = randd( -1.0, 1.0 );
            v2 = randd( -1.0, 1.0 );
            rsq = v1*v1 + v2*v2;
        }
        fac = sqrt( -2.0 * log( rsq ) / rsq );
        lastGauss = v1 * fac;
        fac *= v2;
    }
    else
    {
        fac = lastGauss;
        lastGauss = DBL_MAX;
    }
    return mu + sigma * fac;
}

bool RandMaster::randp( double p )
{
    return ( (double)rand() / RAND_MAX ) < p;
}

Interval::Interval()
    : _lo(0), _hi(0), _loOpen(true), _hiOpen(true){}

Interval::Interval( double lo, double hi )
    : _lo(lo), _hi(hi), _loOpen(true), _hiOpen(true){}

Interval::Interval( double lo, double hi, bool loOpen, bool hiOpen )
    : _lo(lo), _hi(hi), _loOpen(loOpen), _hiOpen(hiOpen){}

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
    double r = _lo - 1.0;
    while( ! in(r) )
    {
        r = R.randg( mu, sigma );
    }
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

string Interval::str()
{
    return string( (_loOpen?"( ":"[ ") + num2str(_lo) + ", " + num2str(_hi) + (_hiOpen?" )":" ]") );
}

ostream& operator<<( ostream& out, Interval& i )
{
    return out << i.str();
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
