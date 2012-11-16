#include "convert_utilities.hpp"

double d2r_factor = PI / 180.0;
double r2d_factor = 1.0 / d2r_factor;



double toRadians( double degrees )
{
    return degrees * d2r_factor;
}



double toDegrees( double radians )
{
    return radians * r2d_factor;
}



int char2Int( const char a )
{
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



void index2coords( int idx, const std::vector<int> &dims, std::vector<int> &coords )
{
    int t = idx;
    coords = std::vector<int>( dims.size(), 0 );
    for( int i=dims.size()-1; i>1; i-- )
        divmod( t, dims[i], t, coords[i] );
    divmod( t, dims[1], coords[0], coords[1] );
}



void coords2index( int &idx, const std::vector<int> &dims, const std::vector<int> &coords )
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
