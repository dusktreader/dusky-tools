#pragma once

#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "tools/local_assert/local_assert.h"
#include "convert_utilities.hpp"

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

#ifndef EPS
#define EPS 1e-10
#endif

cv::Mat crop( const cv::Mat& src, cv::Rect_<int> roi );

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

cv::Mat drawPoints( const cv::Mat& src, std::vector< cv::Point_<double> >& points, int radius=3, int width=1, cv::Scalar color=CV_NRED );

cv::Mat strel( int size );



/** Describes a thin wrapper for the cv::Point_ class that adds some extra functionality */
template <class T>
class PointPlus : public cv::Point_<T>
{
private:
    int _flag;
public:
    /** Creates a PointPlus */
    PointPlus() : cv::Point_<T>(), _flag(0){}

    /** Creates a PointPlus from an (x,y) coordinate */
    PointPlus( T x, T y ) : cv::Point_<T>(x,y), _flag(0){}

    /** Creates a PointPlus from an (x,y) coordinate and a flag*/
    PointPlus( T x, T y, int flag ) : cv::Point_<T>(x,y), _flag(flag){}

    /** Creates a PointPlus from a standard cv::Point_ */
    PointPlus( const cv::Point_<T>& p ) : cv::Point_<T>(p), _flag(0){}

    /** Creates a PointPlus from a standard template cv::Point_ of a different type*/
    template <class P>
    PointPlus( const cv::Point_<P>& p ) : cv::Point_<T>( (T)p.x, (T)p.y ), _flag(0){}

    /** Performs an affine transformation on this PointPlus using a transformation matrix */
    void transform( const cv::Mat& xfm )
    {
        // Creates a matrix equivalent of the point and transforms it
        cv::Mat_<T> pt( cv::Size(1,3) );
        pt[0][0] = this->x;
        pt[1][0] = this->y;
        pt[2][0] = 1;
        pt = xfm * pt;
        this->x = pt[0][0];
        this->y = pt[1][0];
    }

    void xfm( const cv::Mat& t )
    {
        cv::Mat_<T> pt( 3, 1, 1 );
        pt(0,0) = this->x;
        pt(1,0) = this->y;
        pt = t * pt;
        this->x = pt(0,0);
        this->y = pt(1,0);
    }

    /** Calculates the distance between this and another point */
    inline double dist( const PointPlus<T>& other )
    {
        return sqrt( pow( this->x - other.x, 2.0 ) + pow( this->y - other.y, 2.0 ) );
    }

    double operator -( const PointPlus<T>& other )
    {
        return this->dist( other );
    }

    /** Moves this point by a scalar x and y distance */
    void move( T dx, T dy )
    {
        this->x += dx;
        this->y += dy;
    }

    /** Sets the flag for this PointPlus */
    void setFlag( int flag )
    {
        _flag = flag;
    }

    /** Retrieves the flag for this PointPlus */
    int flag()
    {
        return _flag;
    }

    /** Returns a string representation of this PointPlus */
    inline std::string str()
    {
        return std::string( "( " + num2str(this->x) + ", " + num2str(this->y) + " )" );
    }

    /** Allows the string representation of a PointPlus to be placed on an output stream */
    friend std::ostream& operator<<( std::ostream& out, PointPlus& p )
    {
        return out << p.str();
    }
};

/** Describes a thin wrapper for the cv::Size_ class that adds some extra functionality */
template <class T>
class SizePlus : public cv::Size_<T>
{
public:
    /// @note The w and h reference variables are merely to conveniently acces width and height
    T& w;
    T& h;

    /** Creates a SizePlus */
    SizePlus() : cv::Size_<T>(), w(this->width), h(this->height){}

    /** Creates a SizePlus from a width and height */
    SizePlus( T width, T height ) : cv::Size_<T>(width,height), w(this->width), h(this->height){}

    /** Creates a SizePlus from a standard cv::Size_ */
    SizePlus( const cv::Size_<T>& s ) : cv::Size_<T>(s), w(this->width), h(this->height){}

    /** Creates a SizePlus from a standard template cv::Size_ of a different type*/
    template <class S>
    SizePlus( const cv::Size_<S>& s ) : cv::Size_<T>( (T)s.width, (T)s.height ), w(this->width), h(this->height){}

    /** Creates a new SizePlus that is equivalent to this SizePlus scaled by a constant scale factor */
    SizePlus<T> operator*( double scale )
    {
        SizePlus<T> neo = *this;
        neo.w *= scale;
        neo.h *= scale;
        return neo;
    }

    /** Scales this SizePlus by a constant scale factor */
    SizePlus<T>& operator*=( double scale )
    {
        this->w *= scale;
        this->h *= scale;
        return *this;
    }

    /** Returns a string representation of this SizePlus */
    inline std::string str()
    {
        return std::string( "( " + num2str(this->w) + ", " + num2str(this->h) + " )" );
    }

    /** Allows the string representation of a SizePlus to be placed on an output stream */
    friend std::ostream& operator<<( std::ostream& out, SizePlus& s )
    {
        return out << s.str();
    }
};

/** Describes a thin wrapper for the cv::Rect_ class that adds some extra functionality */
template <class T>
class RectPlus : public cv::Rect_<T>
{
public:
    double attribute;

    /** Creates a PointPlus */
    RectPlus() : cv::Rect_<T>(){}

    /** Creates a RectPlus from an (x,y,w,h) rectangle description */
    RectPlus( T x, T y, T w, T h ) : cv::Rect_<T>(x,y,w,h){}

    /** Creates a RectPlus from a standard template cv::Rect_ */
    RectPlus( const cv::Rect_<T>& r ) : cv::Rect_<T>(r){}

    /** Creates a RectPlus from a standard template cv::Rect_ of a different type*/
    template <class R>
    RectPlus( const cv::Rect_<R>& r ) : cv::Rect_<T>( (T)r.x, (T)r.y, (T)r.width, (T)r.height ){}

    /** Creates a RectPlus from an old style CvRect */
    RectPlus( const CvRect& r ) : cv::Rect_<T>(r){}

    /** Creates a RectPlus from a SizePlus object */
    RectPlus( const SizePlus<T>& sz ) : cv::Rect_<T>(0,0,sz.width,sz.height){}

    /** Creates a RectPlus from a cv::Size_ object
    RectPlus( const cv::Size_<T>& sz ) : cv::Rect_<T>(0,0,sz.width,sz.height){}
    */

    /** Creates a RectPlus from an upper-left and lower-left point */
    RectPlus( const cv::Point_<T>& ul, const cv::Point_<T>& lr )
    {
        this->x = ul.x;
        this->y = ul.y;
        this->width = lr.x - ul.x;
        this->height = lr.y - ul.y;
    }

    /** Finds and returns the center of this RectPlus as a PointPlus */
    inline PointPlus<T> center() const
    {
        return PointPlus<T>( this->x + this->width / 2, this->y + this->height / 2 );
    }

    /** Returns a the upper-left corner of this RectPlus */
    inline PointPlus<T> ul() const
    {
        return PointPlus<T>( this->x, this->y );
    }

    /** Returns a the lower-left corner of this RectPlus */
    inline PointPlus<T> ll() const
    {
        return PointPlus<T>( this->x, this->y + this->height );
    }

    /** Returns a the upper-right corner of this RectPlus */
    inline PointPlus<T> ur() const
    {
        return PointPlus<T>( this->x + this->width, this->y );
    }

    /** Returns a the lower-right corner of this RectPlus */
    inline PointPlus<T> lr() const
    {
        return PointPlus<T>( this->x + this->width, this->y + this->height );
    }

    /** Returns the x position of the left side of this RectPlus */
    inline T left() const
    {
        return this->x;
    }

    /** Returns the y position of the top side of this RectPlus */
    inline T top() const
    {
        return this->y;
    }

    /** Returns the x position of the right side of this RectPlus */
    inline T right() const
    {
        return this->x + this->width;
    }

    /** Returns the y position of the bottom side of this RectPlus */
    inline T bottom() const
    {
        return this->y + this->height;
    }

    /** Moves this RectPlus so that it is centered on a point */
    void centerOn( const cv::Point_<T>& pt )
    {
        this->x += pt.x - this->center().x;
        this->y += pt.y - this->center().y;
    }

    /** Anchors this RectPlus's upper-left coner on a point */
    void anchorOn( const cv::Point_<T>& pt )
    {
        this->x = pt.x;
        this->y = pt.y;
    }

    /** Returns a cv::RotatedRect from this rectangle */
    inline void rotated( double angle )
    {
        return cv::RotatedRect( this->center(), this->size(), angle );
    }

    /** Scales this RectPlus by a constant scale factor relative to a designated origin point */
    void scale( double scl, PointPlus<T> origin=PointPlus<T>() )
    {
        this->x = (T)( origin.x + ( this->x - origin.x ) * scl );
        this->y = (T)( origin.y + ( this->y - origin.y ) * scl );
        this->width  = (T)( this->width  * scl );
        this->height = (T)( this->height * scl );
    }

    /** Scales this RectPlus by a constant scale factor relative to its own center */
    RectPlus& operator*=( double scl )
    {
        this->scale( scl, this->center() );
        return *this;
    }

    /** Reports the aspect ratio of this rectangle as width / height */
    double aspect()
    {
        double num = min( this->width, this->height );
        double den = max( this->width, this->height );
        return num / den;
    }

    /** Creates a set of rectangles that sub-divide this rectangle */
    std::vector< RectPlus<T> > subdivide( int xDivs, int yDivs )
    {
        std::vector< RectPlus<T> > rects;
        T dx = this->width / xDivs;
        T dy = this->height / yDivs;
        T x, y;
        for( int i=0; i<yDivs; i++ )
        {
            y = this->y + i * dy;
            for( int j=0; j<xDivs; j++ )
            {
                x = this->x + j * dx;
                rects.push_back( RectPlus<T>( x, y, dx, dy ) );
            }
        }
        return rects;
    }

    T& w()
    {
        return this->width;
    }

    T& h()
    {
        return this->height;
    }

    /** Returns a string representation of this RectPlus */
    inline std::string str()
    {
        return std::string( "[ " + num2str(this->x)     + ", " + num2str(this->y)      + ", " +
                              num2str(this->width) + ", " + num2str(this->height) + " ]" );
    }


    /** Allows the string representation of a RectPlus to be placed on an output stream */
    friend std::ostream& operator<<( std::ostream& out, RectPlus& r )
    {
        return out << r.str();
    }

};
