#include "print_utilities.hpp"

std::string dumpSStream( std::stringstream& stream )
{
    std::string ret = stream.str();
    stream.str( std::string() );
    return ret;
}

std::string wrapText( std::string inputString, int rightMargin,  int leftPadding )
{
    int span = rightMargin - leftPadding;
    int i = 0;
    int j = 0;
    int lineLength = 0;
    if( inputString.length() == 0 )
        return "";
    std::string wrappedString;

    std::string padString;
    for( int i=0; i<leftPadding; i++ )
        padString.push_back( ' ' );

    bool hyphenate = false;

    do
    {
        if( inputString[i] == ' ' )
        {
            i++;
        }
        else
        {
            if( i + span >= (int)inputString.length() )
            {
                hyphenate = false;
                lineLength = inputString.length() - i;
            }
            else
            {
                j = span;
                do
                {
                    if( inputString[ i + j ] == ' ' )
                    {
                        hyphenate = false;
                        lineLength = j;
                        break;
                    }
                    else if( j == 0 )
                    {
                        hyphenate = true;
                        lineLength = span - 1;
                        break;
                    }
                    else
                    {
                        j--;
                    }
                } while( 1 );
            }

            wrappedString += padString;

            wrappedString += inputString.substr( i, lineLength );
            i += lineLength;

            if( hyphenate )
                wrappedString.push_back( '-' );

            wrappedString.push_back( '\n' );
        }
    } while( i < (int)inputString.length() );

    return wrappedString;
}

/** @brief  Indents and Wraps a string within a margin
 *  @todo   Write a c++ version of this function
char* wrap_text_cstr(
    char* str,        ///< The string to reformat
    int right_margin, ///< The right margin to use for wrapping
    int left_padding  ///< The indentation for the text
    )
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
*/
