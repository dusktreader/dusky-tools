#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

/** Dumps the contents of a stringstream to a string and clears the stream
  * @note  This is probably not a good implementation.  Results in 2 copies of the string data.
  */
std::string dumpSStream( std::stringstream& stream );



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

/** Prints a labeled list to an output stream.
 * @note   Any object overloaded for the operator[] using linear access will work
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



/** Prints a labeled matrix to an output stream.
 * @note   The matrix is assumed to be allocated serially in 1 dimension
 *         and matrixPtr is a pointer to the first element
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
void printMatrix( T matrixPtr, int w, int h, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout )
{
    out << label << std::endl
        << "------------" << std::endl;
    for( int i=0; i<h; i++ ){
        if( abbreviate && i == 3 && h > 10 ){
            out << "..." << std::endl;
            i = h - 4;
        }
        printList( matrixPtr + i * w, w, "|", abbreviate, prec, width, out );
    }
}



/** @brief  Indents and Wraps a string within a margin
 *  @todo   Write a c++ version of this function
 */
std::string wrapText(
    std::string inputString,  ///< The string to reformat
    int rightMargin, ///< The right margin to use for wrapping
    int leftPadding  ///< The indentation for the text
    );
