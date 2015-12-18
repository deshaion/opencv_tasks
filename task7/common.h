# pragma once

# ifndef __OPENCV_OLD_CV_H__
	// for the case this headers not included into stdafx.h - do it here

	#include "opencv\cv.h"
	#include "opencv\highgui.h"

	#include <math.h>
	#include <stdlib.h>
	#include <limits>
	#include <set>
	#include <vector>
	#include <deque>
	#include <map>
	#include <iostream>
	#include <functional>
	#include <algorithm>
	#include <utility>
	#include <numeric>
	#include <string>
	
# endif

typedef unsigned __int32 uint32;
typedef unsigned __int32 Uint32;

// 3scattered

int ReadTiffIntoMemory( std::vector<Uint32>& raster, LPCSTR fn ); // return width or (-) code error
int ReadTiffIntoMemory( std::vector<Uint32>& raster, LPCTSTR fn ); // return width or (-) code error

typedef std::vector<byte> MaskType; 
typedef std::deque< MaskType > MaskArrayType;
int SplitMaskPlanes( MaskArrayType& Masks, const std::vector<Uint32>& Raster, const int MaxPlanes );

template <typename T> 
void Decimate( std::vector<T>& out, const std::vector<T> in, int width, const int scale  );

template <typename T> struct At : public std::binary_function< std::vector<T>, int , T > {
	T operator()( const std::vector<T> &dst, const int i ) const { return dst.at(i); }
};
//
template <typename T> 
static inline void Select( std::vector<T> &dst, const std::vector<T> &src, const std::vector<bool> &ind )
{
	Select( dst, src, Index(ind) ) ; 
}
//
template <typename T> 
static void Decimate( std::vector<T>& out, const std::vector<T> in, int width, const int scale  )
{
	const int N=in.size()/(scale*2);
	out.resize(N);
	for( int i=0; i<N; ++i ){
		const int I = i * scale ;
		out[i] = in[ I%width + I/width*2*width ];
	}
}
//
template <typename T> 
static void Select( std::vector<T> &dst, const std::vector<T> &src, const std::vector<int> &ind )
{
	dst.resize( ind.size() );
	std::transform( ind.begin(), ind.end(), dst.begin(), std::bind1st( At<T>(), src ) );
}
//
template<typename T> 
static std::pair<double,double> Std( const T Begin, const T End )
{
	const double s = std::accumulate( Begin, End, double(0) ) ;
	const int N = int( End-Begin );
	const double m = s / N; 
//		double s2 = std::accumulate( Us.begin(), Us.end(), double(0), std::bind2nd( std::ptr_fun<double,int>( pow ) , 2 )  ) ;
	struct sq { double operator()(const double a, const float v) const{ return a + double(v)*v; } }; 
//		std::multiplies<int>
	const double s2 = std::accumulate( Begin, End, double(0), sq() ) ;
	const double std = sqrt( s2/N - m * m  );
	return std::make_pair(m,std);
}
//
template <typename T> 
static inline typename T::value_type Mediana( const T Begin, const T End )
{
	const T m = Begin + ( End - Begin ) / 2;
	std::nth_element( Begin, End, m );
	return m[-1];
}
//
float Mediana( std::vector<float>& v );
int Mediana( std::vector<int>& v );
//
typedef cv::Ptr<IplConvKernel>  StructuringElementType;
static inline StructuringElementType CreateRoundStructuringElement( const int r)
{
	const StructuringElementType ker = cvCreateStructuringElementEx( r, r, r/2, r/2, CV_SHAPE_ELLIPSE );
	return ker; 
}

std::vector<int> Index(const MaskType &b) ;
static inline cv::Point Index2Point( const int width, const int n ){ return cv::Point( n%width, n/width ); }
static inline int Point2Index( const int width, const cv::Point p ){ return p.x + p.y * width; }

void MoveInd( std::vector<int>& dst, const std::vector<int>& src, const cv::Point p , const cv::Size sz, const int bo=0 );
std::vector<int> CreateIndexBall( const cv::Size sz, const int r );
//
template <typename T> 
static inline void SetValue( T* const dst, const std::vector<int> &index, const T value )
{
	for( std::vector<int>::const_iterator i=index.begin(); i != index.end(); ++i ){
		dst[ *i ] = value;
	}
}
//
const std::vector<int> cvCreateAlphaShape(cv::Mat mg,const int r);
const std::vector<int> CreateAlphaShape( cv::Mat mg, const int r, bool show=false );
// cv shells & wrappers
static inline void cvShowImage(const char* win,const cv::Mat& mat){ cvShowImage( win , &CvMat(mat) ); }
static inline void cvShowImage(const char* win,const cv::Size sz, const byte* bp ){ 	cvShowImage( win , &CvMat( cv::Mat( sz, CV_8UC1, const_cast<byte*>(bp) )) ); }

static inline void cvMorphologyEx( const cv::Mat& src, cv::Mat& dst, cv::Mat& tmp, IplConvKernel* element, int operation, int iterations=1 )
{
	cvMorphologyEx( &CvMat(src), &CvMat(dst), &CvMat(tmp), element, operation, iterations );	
}
static inline void cvDilate( const cv::Mat& src, cv::Mat& dst, IplConvKernel* element=NULL, int iterations=1 )
{
	cvDilate( &CvMat(src), &CvMat(dst), element, iterations );	
}

#define ThrowLogicError	{ char b[256]; sprintf_s( b , "logical error in line %d", __LINE__ );  throw std::logic_error(b); }
//
template <typename T> static inline 
std::vector<int> Index( T Begin, T End ) 
{
	std::deque<int> out;
	for( T bi = Begin; bi != End;  ++bi ){
		if( *bi )
			out.push_back( int( bi-Begin ) );
	}
	return std::vector<int>( out.begin(), out.end() );
}

//# define Nelem(a) (sizeof(a)/sizeof(a[0]))
template<typename T> static inline T Ceil(const T v){ return ceil(v); }
template<> static inline int Ceil<int>(const int v){ return v; }
template<typename T> static inline  T Floor(const T v){ return floor(v); }
template<> static inline int Floor<int>(const int v){ return v; }

template <typename T> static inline 
int signum(T x) {
    return T(0) < x;
}
struct pair1
{
	template <typename T>
	typename T::first_type operator()(T keyValuePair) const
	{
		return keyValuePair.first;
	}
};
struct pair2
{
	template <typename T>
	typename T::second_type operator()(T keyValuePair) const
	{
		return keyValuePair.second;
	}
};
typedef pair1 RetrieveKeys;
template <typename T,typename K> 
T Get(std::map<T,K> m,K key){	return m[key];	};

/*
template <typename T> struct Atptr : public std::binary_function< const T*, int , T > {
	T operator()( const T* const dst, const int i ) const { return dst[i]; }
};
*/
template <typename T> inline T fAt( const T* dst, const int i){ return dst[i]; }
//template <typename T> inline T& fAtRef( T* dst, const int i){ return dst[i]; }
template <typename T,typename S> static inline 
void Select( std::vector<T> &dst, const S* src, const std::vector<int> &ind )
{
	dst.resize( ind.size() );
//	std::transform( ind.begin(), ind.end(), dst.begin(), std::bind1st( Atptr<S>(), src ) );
	std::transform( ind.begin(), ind.end(), dst.begin(), std::bind1st( std::ptr_fun( fAt<S>) , src ) );
}
template <typename T> static inline 
std::string toString(T val)
{
    std::ostringstream oss;
    oss<< val;
    return oss.str();
}

 
template<typename T> static inline 
T fromString(const std::string& s)
{
  std::istringstream iss(s);
  T res;
  iss >> res;
  return res;
}

void  GetMat1CanFromIm(IplImage *frame,IplImage *gray_img, CvMat *gray_mat);
 ////////////////////////////////////////////////////////////////

 

