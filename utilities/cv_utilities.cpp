#include "cv_utilities.hpp"

using namespace std;

cv::Mat crop( const cv::Mat& src, cv::Rect_<int> roi )
{
    cv::Mat sub( roi.size(), src.type() );
    sub.setTo( 0 );

    if( roi.br().x <= 0 || roi.br().y <= 0 ||
        roi.tl().x > src.cols || roi.tl().y > src.rows )
        return sub;

    cv::Rect srcRoi, subRoi;
    srcRoi = roi;

    srcRoi.x = max( 0, roi.x );
    srcRoi.y = max( 0, roi.y );

    subRoi.x = srcRoi.x - roi.x;
    subRoi.y = srcRoi.y - roi.y;

    subRoi.width  = srcRoi.width  = min( roi.width  - subRoi.x, src.cols - srcRoi.x );
    subRoi.height = srcRoi.height = min( roi.height - subRoi.y, src.rows - srcRoi.y );

    cv::Mat wrk( sub, subRoi );
    cv::Mat( src, srcRoi ).copyTo( wrk );

    return sub;
}



void makeHisto1D( const cv::Mat& src, vector<double>& hist, int binCt, const cv::Mat& msk )
{
    ASSERT_MSG( src.type() == CV_8UC1, "This method can only use 8 bit single channel images" );
    ASSERT_MSG( msk.type() == CV_8UC1, "Only an 8 bit single channel mask may be used" );

    hist = vector<double>( binCt, 0.0 );
    int N = 0;

    for( int i=0; i<src.rows; i++ )
    {
        for( int j=0; j<src.rows; j++ )
        {
            if( !msk.empty() && msk.at<uchar>(i,j) == 0 )
                continue;
            hist[ src.at<uchar>( i, j ) * binCt / 256 ]++;
            N++;
        }
    }
    for( int i=0; i<binCt; i++ )
    {
        hist[i] /= N;
    }
}

void multiOtsu( const cv::Mat& src, cv::Mat& dst, int M )
{
    int L = 256;
    vector<double> p;
    makeHisto1D( src, p, L );

    cv::Mat_<double> H = cv::Mat_<double>( L, L, 0.0 );
    cv::Mat_<double> P = H.clone();
    cv::Mat_<double> S = H.clone();

    for( int u=0; u<L; u++ )
    {
        for( int v=u; v<L; v++ )
        {
            if( u == 0 )
            {
                if( v ==0 )
                {
                    P( 0, 0 ) = p[0];
                    S( 0, 0 ) = p[0];
                }
                else
                {
                    P( 0, v ) = P( 0, v-1 ) + p[v];
                    S( 0, v ) = S( 0, v-1 ) + v * p[v];
                }
            }
            else
            {
                P( u, v ) = P( 0, v ) - P( 0, u - 1 );
                S( u, v ) = S( 0, v ) - S( 0, u - 1 );
            }
            H( u, v ) = pow( S( u, v ), 2.0 ) / P( u, v );
        }
    }

    int w = L - M,
        h = M - 1;
    int divs[h];
    for( int i=0; i<h; i++ )
        divs[i] = i;
    divs[h-1] = divs[h-2];
    double varMax = 0.0;
    vector< pair<int,int> > tMax;
    while( divs[0] < w )
    {
        int i = h - 1;
        divs[i]++;
        while( divs[i] > i + w )
            divs[--i]++;
        while( ++i < h )
            divs[i] = divs[i-1] + 1;

        vector< pair<int,int> > t;
        t.push_back( pair<int,int>( 0, divs[0] ) );
        for( int i=1; i<M-1; i++ )
            t.push_back( pair<int,int>( divs[i-1]+1, divs[i] ) );;
        t.push_back( pair<int,int>( divs[h-1]+1, L-1 ) );

        double var = 0.0;
        for( unsigned int j=0; j<t.size(); j++ )
            var += H[t[j].first][t[j].second];

        if( var > varMax )
        {
            varMax = var;
            tMax = t;
        }
    }

    vector<int> clrMap(256,0);

    for( int i=0; i<M; i++ )
        for( int j=tMax[i].first; j<=tMax[i].second; j++ )
            clrMap[j] = i * 255 / ( M-1 );

    //printList( clrMap, 256, "clrMap", false );

    dst = cv::Mat( src.size(), CV_8UC1 );
    for( int i=0; i<dst.rows; i++ )
        for( int j=0; j<dst.cols; j++ )
            dst.at<uchar>( i, j ) = clrMap[ src.at<uchar>( i, j ) ];
}

void filterDoG( const cv::Mat& src, cv::Mat& dst, int size0, int size1 )
{
    cv::Mat dog1, dog2;
    cv::GaussianBlur( src, dog1, cv::Size( size0, size0 ), 0 );
    cv::GaussianBlur( src, dog2, cv::Size( size1, size1 ), 0 );
    dst = cv::abs( dog1 - dog2 );
}

cv::Mat drawPoints( const cv::Mat& src, vector< cv::Point2d >& points, int radius, int width, cv::Scalar color )
{
    cv::Mat ptImg;
    if( src.channels() == 1 )
        cv::cvtColor( src, ptImg, CV_GRAY2RGB );
    else
        ptImg = src.clone();
    for( unsigned int i=0; i<points.size(); i++ )
        cv::circle( ptImg, points[i], radius, color, width );
    return ptImg;
}

cv::Mat strel( int size )
{
    return cv::getStructuringElement( cv::MORPH_ELLIPSE, cv::Size(size,size) );
}
