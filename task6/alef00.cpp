#include "alef00.h"
#include <fstream>
#include <conio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include "math.h"
# include "common.h"

using namespace std;
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


alef00::alef00(void)
{
}

alef00::~alef00(void)
{
}
////////////////////////////////////////////////////////////////////////////////////////////
 template <class T> void alef00::visu_array1(T* vect,int qwe,int n_,std::string name)
{
	cvNamedWindow( name.data(),n_);
	
	int scale_x=5;
	int ww;
	float el;
	CvFont myFont;
	CvMat*VISU=0;
cvInitFont(&myFont,CV_FONT_HERSHEY_PLAIN,0.7,1,0,1,8);
	
el=1;
		
VISU= cvCreateMat(  300,scale_x*(qwe+2), CV_8UC3 );
cvZero( VISU);
cvPutText(
	VISU,
	"string",
	cvPoint(10,10),
	&myFont,
	cvScalar(100,200,255));

int curr_,prew_;	
for(ww =1;ww<qwe;ww++)
{
	

curr_=250-(50/el)*max((int) vect[ww],0);
prew_=250-(50/el)*max((int) vect[ww-1],0);

cvLine( VISU,
	   cvPoint(10+scale_x*(ww-1),10+prew_),
	   cvPoint(10+scale_x*(ww),10+curr_),
	   CV_RGB(255,100,30), 2, 2, 0 );
	   
}//for(ww =0;ww<qwe;ww++)	   
	   
	   
	   
	cvShowImage(name.data(), VISU );
cvReleaseMat(&VISU);   
	   
	
	
	
	
}

//////////////////////////////////////////////////////////////////////////////////////
void alef00::visu_array2(float* vect,int qwe,int n_,std::string name,float el)
{
	cvNamedWindow( name.data(),n_);
	
	int scale_x=5;
	int ww;
	 
	CvFont myFont;
	CvMat*VISU=0;
cvInitFont(&myFont,CV_FONT_HERSHEY_PLAIN,0.7,1,0,1,8);
	
 
		
VISU= cvCreateMat(  300,scale_x*(qwe+2), CV_8UC3 );
cvZero( VISU);
cvPutText(
	VISU,
	"string",
	cvPoint(10,10),
	&myFont,
	cvScalar(100,200,255));

int curr_,prew_;	
for(ww =1;ww<qwe;ww++)
{
	

curr_=250- max((int) (*(vect+ ww)*el),0);
prew_=250- max((int)(*(vect+ ww-1)*el),0);

cvLine( VISU,
	   cvPoint(10+scale_x*(ww-1),10+prew_),
	   cvPoint(10+scale_x*(ww),10+curr_),
	   CV_RGB(255,100,30), 2, 2, 0 );
	   
}//for(ww =0;ww<qwe;ww++)	   
	   
	   
	   
	cvShowImage(name.data(), VISU );
cvReleaseMat(&VISU);   
	   
	
	
	
	
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void alef00::visu_vector(CvMat* vect,int n_,std::string name,int routine,float el, int end_)
{
	cvNamedWindow( name.data(),n_);
	bool string_=(vect->rows < vect->cols);
	int scale_x=5;
	int ww;
	 
	CvFont myFont;
	CvMat*VISU=0;
cvInitFont(&myFont,CV_FONT_HERSHEY_PLAIN,0.7,1,0,1,8);
	if(string_)
	//	if(false)
	{
		 
VISU= cvCreateMat(  300,scale_x*(vect->cols+2), CV_8UC3 );
cvZero( VISU);
cvPutText(
	VISU,
	"string",
	cvPoint(10,10),
	&myFont,
	cvScalar(100,200,255));

	}//if(string_)
	else
	{
		 

VISU= cvCreateMat( 300,  min(end_,scale_x*(vect->rows+2)),CV_8UC3 );
cvZero( VISU);
cvPutText(
	VISU,
	"column",
	cvPoint(10,10),
	&myFont,
	cvScalar(100,200,255));
	}//else


int curr_,prew_;

if(string_)
{
for(ww =1;ww<min(vect->cols,end_);ww++)
{
	float yt=CV_MAT_ELEM( *vect, float, 0, ww);
float yt1=CV_MAT_ELEM( *vect, float, 0, ww-1);
if(routine==1)
{
curr_=250- max((int)(el*CV_MAT_ELEM( *vect, float, 0, ww)),0);
prew_=250-max((int)(el*CV_MAT_ELEM( *vect, float, 0, ww-1)),0);
}
if(routine==0)
{
curr_=250- max((int)CV_MAT_ELEM( *vect, BYTE, ww, 0)*el,0);
prew_=250- max((int)CV_MAT_ELEM( *vect, BYTE, ww-1, 0)*el,0);
}
cvLine( VISU,
	   cvPoint(10+scale_x*(ww-1),10+prew_),
	   cvPoint(10+scale_x*(ww),10+curr_),
	   CV_RGB(255,100,30), 2, 2, 0 );
	   
	   
	   
	   
	   
	   
	   
	
	
	
}//for(ww =0;ww<qwe;ww++)	
}
else//not string
{
for(ww =1;ww<min(vect->rows,end_);ww++)
{
	float yt=CV_MAT_ELEM( *vect, float, ww, 0);
float yt1=CV_MAT_ELEM( *vect, float, ww-1, 0);
if(routine==1)
{
curr_=250- max((int)CV_MAT_ELEM( *vect, float, ww, 0)*el,0);
prew_=250- max((int)CV_MAT_ELEM( *vect, float, ww-1, 0)*el,0);
}
if(routine==0)
{
curr_=250- max((int)CV_MAT_ELEM( *vect, BYTE, ww, 0)*el,0);
prew_=250- max((int)CV_MAT_ELEM( *vect, BYTE, ww-1, 0)*el,0);
}
cvLine( VISU,
	   cvPoint(10+scale_x*(ww-1),10+prew_),
	   cvPoint(10+scale_x*(ww),10+curr_),
	   CV_RGB(255,0,30), 2, 2, 0 );
	   
	   
	   
	   
	   
	   
	   
	
	
	
}//for(ww =0;ww<qwe;ww++)
}//else
cvShowImage(name.data(), VISU );
cvReleaseMat(&VISU);
}
/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////
///////////////////////////////////////////////////
void alef00::visu_float(CvMat *mat,string s,int n_)
{
int x_=mat->cols;
int y_=mat->rows;

CvMat *Im2=cvCreateMat( y_,x_,  mat->type );
CvMat *Im1=cvCreateMat( y_,x_,  CV_8UC1 );


double min_val,max_val,diam,scale_;
cvMinMaxLoc( mat,&min_val,&max_val);
diam=max_val-min_val;
scale_= 255.0/diam;
cvSubS(mat,cvScalar(min_val),Im2);
 cvScale(Im2,Im2,scale_);
 cvConvert( Im2 ,Im1);
cvNamedWindow( s.data(),n_);   cvShowImage(s.data(), Im1 ); 

cvReleaseMat(&Im1);
cvReleaseMat(&Im2);
}
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
void  alef00::GetMat1CanFromIm(IplImage *frame,IplImage *gray_img, CvMat *gray_mat)
{
cvCvtColor(frame,gray_img,CV_BGR2GRAY); // 3 cannal -> 1 cannal
cvReshape( gray_img, gray_mat, 1 ); //create matrix
}
////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void  alef00::GetMatNeededCanFrom3DIm(IplImage *frame,  CvMat * mat,int cannal)
{
int	x_=frame->width;
int y_=frame->height;
int I_,J_;
CvMat  Q; cvReshape(frame,&Q,3);
CvScalar  B; 
for( I_=0;I_<y_;I_++)
{
for( J_=0;J_<x_;J_++)
{
	B= cvGet2D(&Q,I_, J_);
	CV_MAT_ELEM(  *mat, BYTE, I_, J_)=(int)B.val[cannal];
}
}

}
////////////////////////////////////////////////////////////////////////////////////

CvMat  alef00::Create_Samples_and_Mediane_Im_fromAVI_(CvMat* MM ,std::string path_avi,int& x_,int& y_,int step,int sum_samples)
{
	CvMat SUBMATR1,SUBMATR2,SUBMATR3,SUBMATR4,buf_1,buf_2;
	CvMat MED_MATR;
	CvMat gray_mat;
	IplImage *frame,*gray_img ;
	
CvCapture* capture = 0;
capture = cvCaptureFromAVI( path_avi.data() );         // load the avi
//capture = cvCreateFileCapture( path_avi.data() )
int t;

bool first=true;
int step_=0; 

int counter=0;
for( t=0;t<step*sum_samples;t++)
        {
 cvGrabFrame( capture );
 frame = cvRetrieveFrame( capture ); 
 if( frame!= NULL ) {
if((t%step)==0)
{

 if(first)
		{
 gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
 step_=gray_img->width;
 first=false;      
}//	if(first)
	
  GetMat1CanFromIm(frame,gray_img, &gray_mat);

cvGetRows(MM,&SUBMATR1,0,gray_mat.rows);
cvGetCols(&SUBMATR1,&SUBMATR2,counter*step_,(counter+1)*step_);

cvCopy(&gray_mat, &SUBMATR2, 0 );





counter++; 
}//if((t%5)==0)
}//if( frame!= NULL ) {
}//for( t=0;t<100;t++)
cvReleaseImage( &gray_img);
cvReleaseImage( &frame);
x_=gray_mat.cols;
y_=gray_mat.rows;
/////////////////////////////////
int I_,J_,ww;
cvGetRows(MM,&SUBMATR1,0,gray_mat.rows);
cvGetCols(&SUBMATR1,&MED_MATR,sum_samples*step_,(sum_samples+1)*step_);
float* bufff=new float[3*sum_samples];
for( I_=0;I_<step_;I_++)
{
for( J_=0;J_<gray_mat.rows;J_++)
{
for(ww =0;ww<sum_samples;ww++)
{
	*(bufff+ww)=CV_MAT_ELEM( *MM, BYTE, J_,ww*step_+I_);
}
          vector<float> fv( bufff, bufff+sum_samples);
		  sort(fv.begin(), fv.end());
int ghg=    (int)Mediana( fv);       
CV_MAT_ELEM( MED_MATR, BYTE, J_,I_)=
ghg;
//(int)fv[2];


}//for( J_=0;J_<gray_mat.rows;J_++)
}//for( I_=0;I_<step_;I_++)
delete[] bufff;

return MED_MATR;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

CvMat  alef00::Create_Samples_and_Mediane_Im_from_AVI_can_YUV(CvMat* MM ,std::string path_avi,int cannal, int& x_,int& y_,int step,int sum_samples)
{
	CvMat SUBMATR1,SUBMATR2,SUBMATR3,SUBMATR4,buf_1,buf_2;
	CvMat MED_MATR;
	CvMat *YUV_;
	 
	IplImage *frame,*img_yuv ;
	
CvCapture* capture = 0;
capture = cvCaptureFromAVI( path_avi.data() );         // load the avi
int t;

bool first=true;
int step_=0; 
 
int counter=0;
for( t=0;t<step*sum_samples;t++)
        {
 cvGrabFrame( capture );
 frame = cvRetrieveFrame( capture ); 
 if( frame!= NULL ) {
if((t%step)==0)
{

 if(first)
		{
  img_yuv = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 3);
  x_ = frame->width;
  y_ = frame->height;
  YUV_=cvCreateMat( y_, x_,CV_8UC1 );
 step_=frame->width;
 first=false;      
}//	if(first)
	 
        cvCvtColor(frame, img_yuv, CV_BGR2YCrCb);
		 GetMatNeededCanFrom3DIm(img_yuv,  YUV_,2);
   

cvGetRows(MM,&SUBMATR1,0,YUV_->rows);
cvGetCols(&SUBMATR1,&SUBMATR2,counter*step_,(counter+1)*step_);

cvCopy(YUV_, &SUBMATR2, 0 );





counter++; 
}//if((t%5)==0)
}//if( frame!= NULL ) {
}//for( t=0;t<100;t++)
cvReleaseImage( &img_yuv);
cvReleaseImage( &frame);

x_=YUV_->cols;
y_=YUV_->rows;
cvReleaseMat(&YUV_);
/////////////////////////////////
int I_,J_,ww;
cvGetRows(MM,&SUBMATR1,y_,2*y_);
cvGetCols(&SUBMATR1,&MED_MATR,sum_samples*step_,(sum_samples+1)*step_);
float* bufff=new float[3*sum_samples];
for( I_=0;I_<step_;I_++)
{
for( J_=0;J_<y_;J_++)
{
for(ww =0;ww<sum_samples;ww++)
{
	*(bufff+ww)=CV_MAT_ELEM( *MM, BYTE, J_,ww*step_+I_);
}
          vector<float> fv( bufff, bufff+sum_samples);
		  sort(fv.begin(), fv.end());
int ghg=    (int)Mediana( fv);       
CV_MAT_ELEM( MED_MATR, BYTE, J_,I_)=
ghg;
//(int)fv[2];


}//for( J_=0;J_<gray_mat.rows;J_++)
}//for( I_=0;I_<step_;I_++)
delete[] bufff;

return MED_MATR;
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
