#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#ifndef PI
#define PI 3.1415926535897931
#endif

#ifndef EPS
#define EPS 1e-10
#endif

/** OS specific flags and etcetera */
#if !defined(WIN32) || defined(__MINGW32__)
#define PRINT_FUNC __PRETTY_FUNCTION__
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#else
#define OS_SEP_STR "\\"
#define OS_SEP_CHAR '\\'
#define PRINT_FUNC __FUNCSIG__
#endif



/** Dumps the contents of a stringstream to a string and clears the stream
  * @note  This is probably not a good implementation.  Results in 2 copies of the string data.
  */
std::string dumpSStream( std::stringstream& stream );



/// @brief  Performs a division and modulo operation
void divmod(
        int dividend,  ///< The numerator of the division
        int divisor,   ///< The denominator of the division
        int &quotient, ///< The result of the division
        int &remainder ///< The result for modulo of the division
        );



/** Calculates the Mean, Standard Deviation, and %RSD of a given group of floating point values
  * @todo   Make this templated to use container interators instead?
  */
void meanStdvPrsd(
        const std::vector<double>& vals, ///< The values for which to compute the statistics
        double& mean,                    ///< A reference to the final mean value
        double& stdv,                    ///< A reference to the final standard deviation value
        double& prsd,                    ///< A reference to the final percent RSD value
        bool nm1d=true                   ///< Use an (n-1) divisor for the standard deviation
        );



/** Prints a labeled variable to an output stream
  * @param  var   - The variable to print
  * @param  label - The label for the variable
  * @param  out   - The output stream
  */
template <class T>
void printVar( T var, std::string label="variable", std::ostream &out=std::cout )
{
    out << label << " = " << var << std::endl;
}

/** Prints a labeled list to an output stream.  Any object overloaded for the operator[] using linear access will work
  * @param  list       - The list to print
  * @param  l          - The number of items to print.  This could overrun the list!
  * @param  label      - The label for the list
  * @param  abbreviate - Will only print the first and last three elements if true.
  * @param  prec       - Floating point precision to use
  * @param  width      - Padding width for numeric types
  * @param  out        - The output stream
  */
template <class T>
void printList( T list, int l, std::string label="items", bool abbreviate=false, int prec=2, int width=0, bool reverse=false, std::ostream &out=std::cout )
{
    out << label << ": [ " << std::flush;
    int i0, i1, di;
    if( reverse )
    {
        i0 = l - 1;
        i1 = -1;
        di = -1;
    }
    else
    {
        i0 = 0;
        i1 = l;
        di = 1;
    }
    for( int i=i0; i!=i1; i+=di )
    {
        if( abbreviate && i == i0 + di*3 && l > 10 )
        {
            out << "... " << std::flush;
            i = i1 + -di * 4;
        }
        else
            out << std::setw( width ) <<
                   std::setfill( ' ' ) <<
                   std::fixed <<
                   std::setprecision( prec ) <<
                   list[i] << " " << std::flush;
    }
    out << " ]" << std::endl;
}



/** Prints a labeled matrix to an output stream.  The matrix is assumed to be allocated serially in 1 dimension
  * @param  matrix     - The matrix to print
  * @param  w          - The width of the matrix
  * @param  h          - The height of the matrix
  * @param  label      - The label for the matrix
  * @param  abbreviate - Will only print the first and last three rows/columns if true.
  * @param  prec       - Floating point precision to use
  * @param  width      - Padding width for numeric types
  * @param  out        - The output stream
  */
template <class T>
void printMatrix( T matrix, int w, int h, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout )
{
    out << label << std::endl
        << "------------" << std::endl;
    for( int i=0; i<h; i++ ){
        if( abbreviate && i == 3 && h > 10 ){
            out << "..." << std::endl;
            i = h - 4;
        }
        printList( matrix + i * w, w, "|", abbreviate, prec, width, out );
    }
}



/// Squares a numerical value
template <class T>
T pow2( const T& val )
{
    return val * val;
}


char* wrap_text( char* str, int right_margin, int left_padding )
{
    int span = right_margin - left_padding;
    int i = 0;
    int k = 0;
    int j = 0;
    int line_len = 0;
    int len = strlen( str );
    if( len == 0 )
        return NULL;
    char* wrap_str = (char*)malloc( len * sizeof(char) );
    int   wrap_len = len;
    bool hyphenate = 0;

    do
    {
        if( str[i] == ' ' )
        {
            i++;
        }
        else
        {
            /// @note:  The +2 is for the newline character and the null-terminating character;
            if( k + right_margin + 2 > wrap_len )
            {
                wrap_len += right_margin + 2;
                wrap_str = (char*)realloc( wrap_str, wrap_len * sizeof( char ) );
            }


            if( i + span >= len )
            {
                hyphenate = 0;
                line_len = len - i;
            }
            else
            {
                j = span;
                do
                {
                    if( str[ i + j ] == ' ' )
                    {
                        hyphenate = 0;
                        line_len = j;
                        break;
                    }
                    else if( j == 0 )
                    {
                        hyphenate = 1;
                        line_len = span - 1;
                        break;
                    }
                    else
                    {
                        j--;
                    }
                } while( 1 );
            }

            memset( wrap_str + k, ' ', left_padding * sizeof( char ) );
            k += left_padding;

            memcpy( wrap_str + k, str + i, line_len * sizeof( char ) );
            k += line_len;
            i += line_len;

            if( hyphenate )
                wrap_str[k++] = '-';

            wrap_str[k++] = '\n';

            wrap_str[k] = '\0';
        }
    } while( i < len );

    return wrap_str;
}
