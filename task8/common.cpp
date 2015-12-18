//
# include "common.h"

std::vector<int> Index(const MaskType &b) 
{
	std::deque<int> out; 
	for( MaskType::const_iterator bi=b.begin(); bi<b.end(); ++bi ){
		if( *bi )
			out.push_back( int (bi-b.begin() ) );
	}
	return std::vector<int>( out.begin(), out.end() );
}

// process object masks

int SplitMaskPlanes( MaskArrayType& Masks, const std::vector<Uint32>& Raster, const int MaxPlanes )
{
	struct CheckBitMask:public std::binary_function< Uint32, Uint32, byte > { byte operator() ( const Uint32 v, const Uint32 mask ) const { return  (v & mask) !=0? 255:0 ; }; };
	const Uint32 rgb_mask=0xFFFFFFL;  
	struct less { const Uint32 mask_; less(Uint32 mask):mask_(mask){ };  bool operator()(const Uint32 a,const Uint32 b) const { return ( mask_ & a ) < ( mask_ & b );  } };
	const Uint32 maxR = *std::max_element( Raster.begin(), Raster.end(), less( rgb_mask ) ) & rgb_mask ;
	Uint32 maxBit=1; while( maxBit<=maxR ){ maxBit<<=1; } maxBit>>=1;
	{
		MaskType mask( Raster.size() );
		std::transform( Raster.begin(), Raster.end(), mask.begin(), std::bind2nd( CheckBitMask(), rgb_mask )  );
		Masks.push_back(mask);
	}
	Uint32 bit_mask=maxBit;
	for( int k=0; k++ < MaxPlanes && bit_mask ;  bit_mask>>=1 ){
		MaskType mask( Raster.size() );
		std::transform( Raster.begin(), Raster.end(), mask.begin(), std::bind2nd( CheckBitMask(), bit_mask )  );
		Masks.push_back(mask);
	}
	return 0;
}

//
void  GetMat1CanFromIm(IplImage *frame,IplImage *gray_img, CvMat *gray_mat)
{
cvCvtColor(frame,gray_img,CV_BGR2GRAY); // 3 cannal -> 1 cannal
cvReshape( gray_img, gray_mat, 1 ); //create matrix
}
float Mediana( std::vector<float>& v )
{
	return Mediana( v.begin(), v.end() );
}
int Mediana( std::vector<int>& v )
{
	return Mediana( v.begin(), v.end() );
}
//
void MoveInd( std::vector<int>& dst, const std::vector<int>& src, const cv::Point p , const cv::Size sz, const int bo )
{
	dst.resize(src.size());	
	struct tr{ 
		const cv::Point  p_	, max_, min_;
		const cv::Size  sz_	;
		const int bo_		;  
		tr( const cv::Point p , const cv::Size sz, const int bo ): p_(p), sz_(sz), bo_(bo), max_( sz.width-1-bo, sz.height-1-bo ), min_(bo,bo)  { }
		int operator()( int ind ) 
		{
			int x = (ind % sz_.width)  + p_.x	;
			int y = (ind / sz_.width)  + p_.y ;
			if 		( x < min_.x )	x = min_.x	;
			else if ( x > max_.x )	x = max_.x	;
			if 		( y < min_.y )	y = min_.y	;
			else if ( y > max_.y )	y = max_.y	;
			return  x + y * sz_.width ;
		}
	};
	std::transform( src.begin(), src.end(), dst.begin(), tr( p, sz, bo ) );	
}
//
std::vector<int> CreateIndexBall( const cv::Size sz, const int r )
{
	std::deque<int>	res;
	const int R=r*2, rr=r*r ;
	for( int y=0; y<R; y++ ){
		for( int x=0; x<R; x++ ){
			if( (x-r)*(x-r) + (y-r)*(y-r) <= rr ){
				res.push_back( Point2Index( sz.width, cv::Point( x,y ) )  );
			}
		}
	}
	return std::vector<int>( res.begin(), res.end() );
}
//
const std::vector<int> cvCreateAlphaShape(cv::Mat mg,const int r)
{
	std::deque<int> IndexBall;
	IplImage* const img=&((IplImage)mg);
	const cv::Scalar_<byte> col(254);
	const int width=mg.size().width;
	for( cv::MatIterator_<byte> nzi=mg.begin<byte>() ; nzi != mg.end<byte>() ; ++nzi ){
		if( *nzi & 1 ) {
			int n = int ( nzi - mg.begin<byte>( ) );
			IndexBall.push_back( n );
			const cv::Point p = Index2Point( width, n );
			cvCircle( img, p, r, col, CV_FILLED );
# ifdef _SHOW_
			cvShowImage( "MGG" , mg );
			cvWaitKey( 0 );	
# endif
		}
	}
	return std::vector<int>( IndexBall.begin(), IndexBall.end() );
}
//
const std::vector<int> CreateAlphaShape( cv::Mat mg, const int r, bool show )
{
	const cv::Point pr(r,r);
	const cv::Size sz(mg.size());
	const std::vector<int> ball = CreateIndexBall ( sz, r ) ;
	std::vector<int> ballM(ball);
	std::deque<int> IndexBall;
	for( cv::MatIterator_<byte> nzi=mg.begin<byte>() ; nzi != mg.end<byte>() ; ++nzi ){
		if( *nzi & 1 ) {
			const int n = int( nzi - mg.begin<byte>( ) );
			IndexBall.push_back( n );
			const cv::Point p = Index2Point( sz.width, n );
			MoveInd( ballM, ball, p - pr,  sz );
			SetValue<byte>( mg.data, ballM, 254 );
			if( show ){
				cvShowImage( "MGG" , mg );
				cvWaitKey( 0 );	
			}
		}
	}
	return std::vector<int>( IndexBall.begin(), IndexBall.end() );
}
//

