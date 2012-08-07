s
#pragma once

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "ml.h"
#include "cvaux.h"
#include "tools.h"
#include "cvtypesplus.hpp"

#define CV_WHIT cv::Scalar(  255, 255, 255 )
#define CV_NRED cv::Scalar(    0,   0, 255 )
#define CV_LRED cv::Scalar(   50,  50, 255 )
#define CV_DRED cv::Scalar(    0,   0, 205 )
#define CV_NGRN cv::Scalar(    0, 255,   0 )
#define CV_LGRN cv::Scalar(   50, 255,  50 )
#define CV_NBLU cv::Scalar(  255,   0,   0 )
#define CV_DGRN cv::Scalar(    0, 205,   0 )
#define CV_LBLU cv::Scalar(  255,  50,  50 )
#define CV_DBLU cv::Scalar(  205,   0,   0 )
#define CV_GY50 cv::Scalar(  127, 127, 127 )
#define CV_BLAK cv::Scalar(    0,   0,   0 )

#define SHOWW( img )   { cv::namedWindow( #img, 0 ), cv::imshow( #img, img ); cv::waitKey(0); }
#define SHOWN( img ) {  cv::namedWindow( #img, 0 ), cv::imshow( #img, img ); }

#ifndef EPSILON
#define EPSILON 1e-10
#endif

cv::Mat crop( const cv::Mat& src, const RectPlus<int>& roi );

void tophat1D( const cv::Mat& src, cv::Mat& dst, int crownW=10, int brimW=5, int height=3, bool overX=true, bool overY=false, bool invert=false, bool norm=false, const cv::Mat& msk=cv::Mat() );


template<class T>
void printCVMat( cv::Mat_<T> M, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout ){
    out << label << std::endl
        << "------------" << std::endl;
    for( int i=0; i<M.rows; i++ )
    {
        if( abbreviate && i == 3 && M.rows > 10 )
        {
            out << "..." << std::endl;
            i = M.rows - 4;
        }
        out << "| " << std::flush;
        for( int j=0; j<M.cols; j++ )
        {
            if( abbreviate && j == 3 && M.cols > 10 )
            {
                out << "..." << std::flush;
                j = M.cols - 4;
            }
            out << M(i,j) << " " << std::flush;
        }
        out << "|" << std::endl;
    }
}


/** Prints the file, line, matrix name, and matrix values
  * @param  mat - The matrix to report
  */
#define DB_REP_CVMAT( mat )      { std::ostringstream op; \
                                   op << __FILE__ ":" << __LINE__ << ":" << endl; \
                                   printCVMat( mat, #mat, true, 2, 0, op ); \
                                   std::cout << op.str() << std::flush; }


void makeHisto1D( const cv::Mat& src, std::vector<double>& hist, int binCt, const cv::Mat& msk=cv::Mat() );

void multiOtsu( const cv::Mat& src, cv::Mat& dst, int M=4 );

void filterDoG( const cv::Mat& src, cv::Mat& dst, int size0, int size1 );

cv::Mat drawPoints( const cv::Mat& src, std::vector< PointPlus<double> >& points, int radius=3, int width=1, cv::Scalar color=CV_NRED );

cv::Mat strel( int size );
