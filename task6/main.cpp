//
 
#include "common.h"
#include "ini.h"
#include <fstream>
#include <conio.h>
#include <sstream>
#include "alef00.h"
#include "connectedcomponents.h"
#include <queue>
using namespace std;

template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss<< val;
    return oss.str();
}
template<typename T>
T fromString(const std::string& s)
{
  std::istringstream iss(s);
  T res;
  iss >> res;
  return res;
}

string path,path_out,lean,lean_mask;
int max_len;
int routine_;
float thr_;

void foo1()
{
	 float med;
	 int show0=0;
	 //CString s,
	 alef00 al_;
	 int x_,y_;
	 //path=string("C:\\STORE\\XX\\FromHQ\\40_Black(FireWork)_29.97p_1.avi");
     //path_out=string("C:\\STORE\\XX\\FromHQ\\out_00");
	 
	  CreateDirectory( path_out.c_str()   ,NULL);	
	//////////////////////
	if(show0) 
	{
		cvNamedWindow( "filmik",1);   // create the image window 
   cvNamedWindow( "resize",2);   // create the image window 
   cvNamedWindow( "mask",2);   // create the image window 
  cvNamedWindow( "grr",6);   // create the image window
	}
   
// Structure for getting video from camera or avi
     bool first=true;
 
	CvCapture* capture = 0;
     capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *frame, *frame_copy= 0;
	 IplImage gray_img_hdr,gray_img_hdr1,gray_img_hdr_r, *gray_img, *gray_img_r;
	 IplImage *field_img_=0;
	 IplImage *part_img=0;
	 IplImage *part_img_r=0;
     CvMat field_mat,*small_,*small_f;
     double tr_comp;
	 CvMat gray_mat,gray_mat_r,matr_,SUBMATR1,SUBMATR2,SUBMATR1r,SUBMATR2r; 
	 
	 IplImage * histImage = cvCreateImage (cvSize (820,200), 8, 1);
     IplImage * histImage_r = cvCreateImage (cvSize (820,200), 8, 1);
    //int hist_size[] = {200, 200}; 
     int hist_size = 50;// color gradation
     float hranges[] = {0, 255};// color segment
	 float* ranges[] = { hranges };
     CvHistogram * hist = cvCreateHist (1, & hist_size, CV_HIST_ARRAY, ranges, 1); 
     CvHistogram * hist_r = cvCreateHist (1, & hist_size, CV_HIST_ARRAY, ranges, 1); 
     CvMat* field_mat_; 
     int qi,qj;
for(int t=1;t<max_len;t++)
        {
        cvGrabFrame( capture );
       frame = cvRetrieveFrame( capture );         
 if( frame->origin == IPL_ORIGIN_TL )
	  {
	if(first)
		{
         gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         small_f=cvCreateMat(50,50,CV_32FC1);
		 small_=cvCreateMat(50,50,CV_8UC1);
         frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
         cvCopy( frame, frame_copy, 0 );
		 field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
		  al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
		  x_=gray_mat_r.cols;
          y_=gray_mat_r.rows;
		  field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
		  
         first=false;
		}
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
       	cvReshape( field_img_, &field_mat, 1 ); //create matrix	
		cvShowImage("filmik", frame);  // display it
        cvShowImage("grr", gray_img);  // display other
        lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
		lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
        cvResize(&gray_mat,small_,CV_INTER_LINEAR);
        cvConvert(small_,small_f);
		 if(show0) cvShowImage("resize", small_f);  // display it
         float *data_=(float*)(small_f->data.i );
         vector<float> fv( data_, data_+small_->cols*small_->rows);
		 sort(fv.begin(), fv.end());
		 med=Mediana( fv);




		cvZero( field_mat_); 
       for(int qi= 0; qi<y_ ;qi++)
   {
	   for(int pi= 0;pi<x_ ;pi++)
   {
	   if(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-med)>thr_)
	   {
		   CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=255;

	   }
	   }

	   }
    cvShowImage("mask", field_mat_);
     cvSaveImage(lean.data(),&gray_mat);
     cvSaveImage(lean_mask.data(),field_mat_);
      if(show0)   cvWaitKey(0);

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)
 
 cvReleaseImage( &gray_img);
	    cvReleaseImage( &frame);

   	 


	getchar();
}
///////////////////////////////////////////////////////////////////
 void foo2()// adapted thresholding
{
	 float med;
	 int x_0=50;
	 int y_0=50;
	 float *buff=new float[x_0*y_0*2];
	 //CString s,
	 alef00 al_;
	 int x_,y_;
	 CvMat *Diff=cvCreateMat(1,x_0*y_0-1 ,CV_32FC1);
	 cvZero(Diff);
	 //path=string("C:\\STORE\\XX\\FromHQ\\40_Black(FireWork)_29.97p_1.avi");
     //path_out=string("C:\\STORE\\XX\\FromHQ\\out_00");
	 
	  CreateDirectory( path_out.c_str()   ,NULL);	
	//////////////////////
	cvNamedWindow( "filmik",1);   // create the image window 
   cvNamedWindow( "resize",2);   // create the image window 
  cvNamedWindow( "grr",6);   // create the image window 
   int jk;
// Structure for getting video from camera or avi
     bool first=true;
 
	CvCapture* capture = 0;
     capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *frame, *frame_copy= 0;
	 IplImage gray_img_hdr,gray_img_hdr1,gray_img_hdr_r, *gray_img, *gray_img_r;
	 IplImage *field_img_=0;
	 IplImage *part_img=0;
	 IplImage *part_img_r=0;
     CvMat field_mat,*small_,*small_f;
     double tr_comp;
	 CvMat gray_mat,gray_mat_r,matr_,SUBMATR1,SUBMATR2,SUBMATR1r,SUBMATR2r; 
	 
	  
     CvMat* field_mat_; 
     int qi,qj;
for(int t=1;t<max_len;t++)
        {
        cvGrabFrame( capture );
       frame = cvRetrieveFrame( capture );         
 if( frame->origin == IPL_ORIGIN_TL )
	  {
	if(first)
		{
         gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         small_f=cvCreateMat(x_0,y_0,CV_32FC1);
		 small_=cvCreateMat(x_0,y_0,CV_8UC1);
         frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
         cvCopy( frame, frame_copy, 0 );
		 field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
		  al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
		  x_=gray_mat_r.cols;
          y_=gray_mat_r.rows;
		  field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
		  
         first=false;
		}
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
       	cvReshape( field_img_, &field_mat, 1 ); //create matrix	
		cvShowImage("filmik", frame);  // display it
        cvShowImage("grr", gray_img);  // display other
        lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
		lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
        cvResize(&gray_mat,small_,CV_INTER_LINEAR);
        cvConvert(small_,small_f);
		  cvShowImage("resize", small_f);  // display it
          float *data_=(float*)(small_f->data.i );


		  //al_.visu_array (data_,2500,6,string("array"));
		  vector<float> fv( data_, data_+small_->cols*small_->rows);
		  sort(fv.begin(), fv.end());
          med=Mediana( fv);
          float *pfv=&(fv.at(0));
          
          int b_=(fv.size()-1);
		  float sigma=50.4;
		  float power_= 3;
         for ( jk=0;jk< b_+1;jk++)
		 {
          *(buff+jk)= 1-exp(
							    
								  - pow(  abs(*(pfv+jk)- med )  ,power_) /pow( sigma   ,power_)
								   
								   
								  );
           
		 }
		// al_.visu_array2 (buff,x_0*y_0,4,string("array"),10 );
		 
		 for ( jk=0;jk< b_;jk++)
		 {
           
          CV_MAT_ELEM(  *Diff, float, 0,jk)=fabs(*(buff+jk)-*(buff+jk+1));
		 }

         double min_,max_;
		 CvPoint LocMin_,LocMax_;
		 cvMinMaxLoc(Diff,&min_,&max_,&LocMin_,&LocMax_);
 		 al_.visu_vector(Diff,4,string("diff"),1,300  ,2500);
		    al_.visu_array2 (buff,x_0*y_0,4,string("buff"),100 );
           al_.visu_array2 (pfv,x_0*y_0,8,string("pfv"),0.8 );
		int index_=LocMax_.x;
         float thr_0=abs(*(pfv+index_+1)- med);



		cvZero( field_mat_); 
       for(int qi= 0; qi<y_ ;qi++)
   {
	   for(int pi= 0;pi<x_ ;pi++)
   {
	   if(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-med)>thr_0)
	   {
		   CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=255;

	   }
	   }
	   }

     cvSaveImage(lean.data(),&gray_mat);
     cvSaveImage(lean_mask.data(),field_mat_);
        cvWaitKey(0);

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)
 cvReleaseImage( &gray_img);
	    cvReleaseImage( &frame);

   	 

delete [] buff;
	getchar();
}
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
 void foo3()// mediane via time
{
 alef00 al_;
 CvMat SUBMATR1,SUBMATR2,SUBMATR3,SUBMATR4,SUBMATR5,SUBMATR6,SUBMATR7,SUBMATR8,buf_1,buf_2,MED_MATR,gray_mat,gray_mat_r,matr_, SUBMATR1r,SUBMATR2r;
int sum_samples=max_len;
CvMat* MM= cvCreateMat( 2000, 2000*(sum_samples+1), CV_8UC1 );
int x_,y_;
MED_MATR=al_.Create_Samples_and_Mediane_Im_fromAVI_(MM,path,x_,y_,1,sum_samples);
al_.visu_float(&MED_MATR,string("mediana"),1);
 
 cvWaitKey(0);
//delete MM;

	 float med;
	 int x_0=50;
	 int y_0=50;
	 float *buff=new float[x_0*y_0*2];
	 //CString s,
	
	  
	 CvMat *Diff=cvCreateMat(1,x_0*y_0-1 ,CV_32FC1);
	 cvZero(Diff);
	 //path=string("C:\\STORE\\XX\\FromHQ\\40_Black(FireWork)_29.97p_1.avi");
     //path_out=string("C:\\STORE\\XX\\FromHQ\\out_00");
	 
	  CreateDirectory( path_out.c_str()   ,NULL);	
	//////////////////////
	cvNamedWindow( "filmik",1);   // create the image window 
   cvNamedWindow( "resize",2);   // create the image window 
  cvNamedWindow( "grr",6);   // create the image window 
   int jk;
// Structure for getting video from camera or avi
     bool first=true;
 
	CvCapture* capture = 0;
     capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *frame, *frame_copy= 0;
	 IplImage gray_img_hdr,gray_img_hdr1,gray_img_hdr_r, *gray_img, *gray_img_r;
	 IplImage *field_img_=0;
	 IplImage *part_img=0;
	 IplImage *part_img_r=0;
     CvMat field_mat,*small_,*small_f;
     double tr_comp;
	  
	  
     CvMat* field_mat_; 
     int qi,qj;
for(int t=1;t<max_len;t++)
        {
        cvGrabFrame( capture );
       frame = cvRetrieveFrame( capture );         
 if( frame->origin == IPL_ORIGIN_TL )
	  {
	if(first)
		{
         gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         small_f=cvCreateMat(x_0,y_0,CV_32FC1);
		 small_=cvCreateMat(x_0,y_0,CV_8UC1);
         frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
         cvCopy( frame, frame_copy, 0 );
		 field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
		  al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
		  x_=gray_mat_r.cols;
          y_=gray_mat_r.rows;
		  field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
		  
         first=false;
		}
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
       	cvReshape( field_img_, &field_mat, 1 ); //create matrix	
		cvShowImage("filmik", frame);  // display it
        cvShowImage("grr", gray_img);  // display other
        lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
		lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
        cvResize(&gray_mat,small_,CV_INTER_LINEAR);
        cvConvert(small_,small_f);
		  cvShowImage("resize", small_f);  // display it
          float *data_=(float*)(small_f->data.i );


		  //al_.visu_array (data_,2500,6,string("array"));
		  vector<float> fv( data_, data_+small_->cols*small_->rows);
		  sort(fv.begin(), fv.end());
          med=Mediana( fv);
          float *pfv=&(fv.at(0));
          
          int b_=(fv.size()-1);
		  float sigma=50.4;
		  float power_= 3;
         for ( jk=0;jk< b_+1;jk++)
		 {
          *(buff+jk)= 1-exp(
							    
								  - pow(  abs(*(pfv+jk)- med )  ,power_) /pow( sigma   ,power_)
								   
								   
								  );
           
		 }
		// al_.visu_array2 (buff,x_0*y_0,4,string("array"),10 );
		 
		 for ( jk=0;jk< b_;jk++)
		 {
           
          CV_MAT_ELEM(  *Diff, float, 0,jk)=fabs(*(buff+jk)-*(buff+jk+1));
		 }

         double min_,max_;
		 CvPoint LocMin_,LocMax_;
		 cvMinMaxLoc(Diff,&min_,&max_,&LocMin_,&LocMax_);
 		 /*al_.visu_vector(Diff,4,string("diff"),1,300  ,2500);
		    al_.visu_array2 (buff,x_0*y_0,4,string("buff"),100 );
           al_.visu_array2 (pfv,x_0*y_0,8,string("pfv"),0.8 );*/
		int index_=LocMax_.x;
         float thr_0=abs(*(pfv+index_+1)- med);



		cvZero( field_mat_); 
       for(int qi= 0; qi<y_ ;qi++)
   {
	   for(int pi= 0;pi<x_ ;pi++)
   {
	   if(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-(float)CV_MAT_ELEM(  MED_MATR, BYTE, qi, pi))>thr_ )
	   {
		   CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=1;

	   }
	   }
	   }


	   ///////////////
    float thr_ = 1.5;
    bool show  = true;
    
	CvMat* FG0 = inpaint_milk_sauf02(field_mat_, gray_img, thr_, show);
	

	   //////////////
     cvSaveImage(lean.data(),&gray_mat);
     cvSaveImage(lean_mask.data(),field_mat_);
      //  cvWaitKey(0);

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)
 cvReleaseImage( &gray_img);
	    cvReleaseImage( &frame);

   	 

delete [] buff;
	getchar();
}
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
 void foo4()// mediane via time
{
	int show0=0;
	int cannalYUV=2;
	int step=1 ;
	alef00 al_;
	CvMat SUBMATR1,SUBMATR2,SUBMATR3,SUBMATR4,SUBMATR5,SUBMATR6,SUBMATR7,SUBMATR8,buf_1,buf_2,MED_MATR,MED_MATR_U,gray_mat,gray_mat_r,matr_, SUBMATR1r,SUBMATR2r;
	int sum_samples=max_len;
	CvMat* MM= cvCreateMat( 3000, 2000*(sum_samples+1), CV_8UC1 );
	cvZero(MM);
	int x_,y_;

	MED_MATR=al_.Create_Samples_and_Mediane_Im_fromAVI_(MM,path,x_,y_,step,(int)(sum_samples/step));
	MED_MATR_U=al_.Create_Samples_and_Mediane_Im_from_AVI_can_YUV(MM,path,cannalYUV,x_,y_,step,(int)(sum_samples/step));
	if(show0) {
		al_.visu_float(&MED_MATR,string("mediana"),1);
		al_.visu_float(&MED_MATR_U,string("mediana U"),7);
	}

	float med;
	int x_0=50;
	int y_0=50;
	float *buff=new float[x_0*y_0*2];
	//CString s,
		  
	CvMat *Diff=cvCreateMat(1,x_0*y_0-1 ,CV_32FC1);
	cvZero(Diff);
	//path=string("C:\\STORE\\XX\\FromHQ\\40_Black(FireWork)_29.97p_1.avi");
    //path_out=string("C:\\STORE\\XX\\FromHQ\\out_00");
	 
	 CreateDirectory( path_out.c_str()   ,NULL);	
	//////////////////////
	  
	if(show0){
		cvNamedWindow( "filmik",1);   // create the image window 
		cvNamedWindow( "resize",2);   // create the image window 
		cvNamedWindow( "grr",6);   // create the image window 
		cvNamedWindow( "mask_",0); 
		cvNamedWindow( "mask_morph_",0);
		cvNamedWindow( "yuv",2);
		cvNamedWindow( "u",2);
	}
	int jk;
	
	// Structure for getting video from camera or avi
    bool first=true;
    
	CvCapture* capture = 0;
    capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *frame, *frame_copy= 0;
	IplImage gray_img_hdr,gray_img_hdr1,gray_img_hdr_r, *gray_img, *gray_img_r;
	IplImage *field_img_=0;
	IplImage *part_img=0;
	IplImage *part_img_r=0;
    CvMat field_mat,*small_,*small_f;
    double tr_comp;
	IplConvKernel* element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_ELLIPSE );
    CvMat  *YUV_;
	IplImage *img_yuv;
    CvMat *field_mat_, *field_mat_morph_,*field_mat_buff_; 
    int qi,qj;
	
	for(int t=1;t<max_len;t++){
		cvGrabFrame( capture );
		frame = cvRetrieveFrame( capture );         
		if( frame->origin == IPL_ORIGIN_TL ){
			if(first){
				gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				img_yuv=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 3);
				gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				small_f=cvCreateMat(x_0,y_0,CV_32FC1);
				small_=cvCreateMat(x_0,y_0,CV_8UC1);
				frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
				cvCopy( frame, frame_copy, 0 );
				field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
				x_=gray_mat_r.cols;
				y_=gray_mat_r.rows;
				field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
				field_mat_morph_ =cvCreateMat( y_, x_,CV_8UC1 );
				field_mat_buff_ =cvCreateMat( y_, x_,CV_8UC1 );
				YUV_=cvCreateMat( y_, x_,CV_8UC1 );
				first=false;
			}
	    
			al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
			cvCvtColor(frame, img_yuv, CV_BGR2YCrCb);

			al_.GetMatNeededCanFrom3DIm(img_yuv,  YUV_,cannalYUV);
			if(show0){
				cvShowImage("yuv",  img_yuv);
				cvShowImage("u",  YUV_);
			}
       		
			cvReshape( field_img_, &field_mat, 1 ); //create matrix	
			


			
			lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
			lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
			cvResize(&gray_mat,small_,CV_INTER_LINEAR);
			cvConvert(small_,small_f);
			
			if(show0)
			{
			cvShowImage("resize", small_f);  // display it
			cvShowImage("filmik", frame);  // display it
			cvShowImage("grr", gray_img);  // display other
			}
			float *data_=(float*)(small_f->data.i );

			//al_.visu_array (data_,2500,6,string("array"));
			vector<float> fv( data_, data_+small_->cols*small_->rows);
			sort(fv.begin(), fv.end());
			med=Mediana( fv);
			float *pfv=&(fv.at(0));
          
			int b_=(fv.size()-1);
			float sigma=50.4;
			float power_= 3;
			for ( jk=0;jk< b_+1;jk++){
				*(buff+jk)= 1-exp(- pow(  abs(*(pfv+jk)- med )  ,power_) /pow( sigma   ,power_));
           
			}
			// al_.visu_array2 (buff,x_0*y_0,4,string("array"),10 );
		 
			for ( jk=0;jk< b_;jk++){
				CV_MAT_ELEM(  *Diff, float, 0,jk)=fabs(*(buff+jk)-*(buff+jk+1));
			}

			double min_,max_;
			CvPoint LocMin_,LocMax_;
			cvMinMaxLoc(Diff,&min_,&max_,&LocMin_,&LocMax_);
			if(show0){
 				al_.visu_vector(Diff,4,string("diff"),1,300  ,2500);
				al_.visu_array2 (buff,x_0*y_0,4,string("buff"),100 );
				al_.visu_array2 (pfv,x_0*y_0,8,string("pfv"),0.8 ); 
			}
			int index_=LocMax_.x;
			float thr_0=abs(*(pfv+index_+1)- med);
			
			cvSet( field_mat_,cvScalar(1)); 
			for(int qi= 0; qi<y_ ;qi++){
				for(int pi= 0;pi<x_ ;pi++){
					if(
						(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-(float)CV_MAT_ELEM(  MED_MATR, BYTE, qi, pi))>thr_ )
						||
						(abs((float)CV_MAT_ELEM(  *YUV_, BYTE, qi, pi)-(float)CV_MAT_ELEM(  MED_MATR_U, BYTE, qi, pi))>1.5*thr_ )
					)
					{
						CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=0;

						//*(three_channel_mat.data.ptr  + (size_t)three_channel_mat.step * qi + 3 * pi + 0) 
						//*(three_channel_mat.data.ptr  + (size_t)three_channel_mat.step * qi + 3 * pi + 1)
						//*(three_channel_mat.data.ptr  + (size_t)three_channel_mat.step * qi + 3 * pi + 2)

				}
			}
	   }
	///////////////
	cvMorphologyEx(
	field_mat_,
	field_mat_morph_,
	field_mat_buff_,
	element,
	CV_MOP_OPEN );
	//CV_MOP_CLOSE );
	  

    float thr_ = 0.5;
    bool show_  = 0;
    
	CvMat* FG0 = inpaint_milk_sauf02(field_mat_morph_, gray_img, thr_, show_);
	cvSet( field_mat_buff_,cvScalar(1)); 

	cvSub(field_mat_buff_,FG0,field_mat_morph_);
	cvReleaseMat( &FG0);
    cvScale(field_mat_morph_,field_mat_ , 255);
	ConnectedComponents* CC = new ConnectedComponents(field_mat_morph_);
    //CvMat* result = CC->drawComponents(20,false  ); 
	 CvMat* morph_delete_small_ = CC->drawComponents(5,true  ); 
	  if(show0)
	  {
   cvNamedWindow("6531");cvShowImage("6531", field_mat_);
   cvNamedWindow("6534");cvShowImage("6534", morph_delete_small_);
	 cvWaitKey(0);
	  }
       
	   //////////////
 
 cvSaveImage(lean_mask.data(),morph_delete_small_);
cvReleaseMat(&morph_delete_small_);
delete CC;
//cvMorphologyEx(
//field_mat_morph_,
//field_mat_,
//field_mat_buff_,
//element,
//CV_MOP_OPEN );
////CV_MOP_CLOSE );
    
    
     cvSaveImage(lean.data(),&gray_mat);
     
if(show0) cvWaitKey(0);
      

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)

 cvReleaseImage( &gray_img);
	    cvReleaseImage( &frame);

   	 

delete [] buff;
	getchar();
}
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
 void foo5()// mediane local
{
	float med;
	 int show0=0;
	 //CString s,
	 alef00 al_;
	 int x_,y_;
	 //path=string("C:\\STORE\\XX\\FromHQ\\40_Black(FireWork)_29.97p_1.avi");
     //path_out=string("C:\\STORE\\XX\\FromHQ\\out_00");
	 
	  CreateDirectory( path_out.c_str()   ,NULL);	
	//////////////////////
	if(show0) 
	{
		cvNamedWindow( "filmik",1);   // create the image window 
   cvNamedWindow( "resize",2);   // create the image window 
   cvNamedWindow( "mask",2);   // create the image window 
  cvNamedWindow( "grr",6);   // create the image window
	}
   
// Structure for getting video from camera or avi
     bool first=true;
 
	CvCapture* capture = 0;
     capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *frame, *frame_copy= 0;
	 IplImage gray_img_hdr,gray_img_hdr1,gray_img_hdr_r, *gray_img, *gray_img_r;
	 IplImage *field_img_=0;
	 IplImage *part_img=0;
	 IplImage *part_img_r=0;
     CvMat field_mat,*small_,*small_f;
     double tr_comp;
	 CvMat gray_mat,gray_mat_r,matr_,SUBMATR1,SUBMATR2,SUBMATR1r,SUBMATR2r; 
	 
	 IplImage * histImage = cvCreateImage (cvSize (820,200), 8, 1);
     IplImage * histImage_r = cvCreateImage (cvSize (820,200), 8, 1);
    //int hist_size[] = {200, 200}; 
     int hist_size = 50;// color gradation
     float hranges[] = {0, 255};// color segment
	 float* ranges[] = { hranges };
     CvHistogram * hist = cvCreateHist (1, & hist_size, CV_HIST_ARRAY, ranges, 1); 
     CvHistogram * hist_r = cvCreateHist (1, & hist_size, CV_HIST_ARRAY, ranges, 1); 
     CvMat *field_mat_,*field_mat_buff_; 
     int qi,qj;
for(int t=1;t<max_len;t++)
        {
        cvGrabFrame( capture );
       frame = cvRetrieveFrame( capture );         
 if( frame->origin == IPL_ORIGIN_TL )
	  {
	if(first)
		{
         gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         small_f=cvCreateMat(50,50,CV_32FC1);
		 small_=cvCreateMat(50,50,CV_8UC1);
         frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
         cvCopy( frame, frame_copy, 0 );
		 field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
		  al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
		  x_=gray_mat_r.cols;
          y_=gray_mat_r.rows;
		  field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
		  field_mat_buff_ =cvCreateMat( y_, x_,CV_8UC1 );
		  
         first=false;
		}
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
       	cvReshape( field_img_, &field_mat, 1 ); //create matrix	
		cvShowImage("filmik", frame);  // display it
        cvShowImage("grr", gray_img);  // display other
        lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
		lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
            cvSmooth( &gray_mat, field_mat_buff_, CV_MEDIAN ,21 );
			 if(show0) 
	         {
            cvNamedWindow("6531");cvShowImage("6531", &gray_mat);
            cvNamedWindow("6534");cvShowImage("6534", field_mat_buff_);
			 }


        cvResize(&gray_mat,small_,CV_INTER_LINEAR);
        cvConvert(small_,small_f);
		 if(show0) cvShowImage("resize", small_f);  // display it
         float *data_=(float*)(small_f->data.i );
         vector<float> fv( data_, data_+small_->cols*small_->rows);
		 sort(fv.begin(), fv.end());
		 med=Mediana( fv);




		cvZero( field_mat_); 
       for(int qi= 0; qi<y_ ;qi++)
   {
	   for(int pi= 0;pi<x_ ;pi++)
   {
	   if(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-(float)CV_MAT_ELEM(  *field_mat_buff_, BYTE, qi, pi))>thr_)
	   {
		   CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=255;

	   }
	   }

	   }
   	 imfill(field_mat_);
     cvSaveImage(lean.data(),&gray_mat);
     cvSaveImage(lean_mask.data(),field_mat_);
	  

      if(show0) 
	  {
		   cvShowImage("mask", field_mat_);
		  cvWaitKey(0);
	  }

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)
 
 cvReleaseImage( &gray_img);
	    cvReleaseImage( &frame);

   	 


	getchar();
}

////////////////////////////////////////////////////////////////

class mediana{
	vector< vector<int> > data;
	vector<int> dataMed;
	IplImage* gray_img;
	int x, y, n;
	
public:  
	mediana(int medSize){
		n = medSize;
		x = 0;
		y = 0;
	}
	void destroy(){
		cvReleaseImage(& gray_img);
		dataMed.clear();
		data.clear();
	}
	CvMat get(int ID){
		CvMat* MED_MATR = cvCreateMat(x, y, CV_8UC1);
		cvZero(MED_MATR);

		for (int i = 0; i < x; ++i){
			for (int j = 0; j < y; ++j){
				vector<int> fv;
				fv.resize(data[i*y+j].size());
				copy(data[i*y+j].begin(), data[i*y+j].end(), fv.begin());
				sort(fv.begin(), fv.end());
				int med = Mediana(fv);
				setCellM(MED_MATR, i, j, med);
			}
		}

		return *MED_MATR;
	}
	void addNext(IplImage* img){
		bool first = false;
		if (x == 0) {
			x = img->height;
			first = true;
		}
		if (y == 0) {
			y = img->width;
			gray_img = cvCreateImage( cvSize(y, x),IPL_DEPTH_8U, 1);
		}
		cvCvtColor(img, gray_img, CV_BGR2GRAY); // 3 cannal -> 1 cannal
		
		for (int i = 0; i < x; ++i){
			for (int j = 0; j < y; ++j){
				int t = getCellF(gray_img, i, j);
				
				if (first) {
					vector<int> vt;
					vt.push_back(t);
					data.push_back(vt);
				} else {
					data[i*y+j].insert(data[i*y+j].begin(), t);

					if (data[i*y+j].size() > (n * 2 + 1)) data[i*y+j].pop_back();
				}
			}
		}
	}
	void addNext(){
		for (int i = 0; i < x; ++i){
			for (int j = 0; j < y; ++j){
				data[i*y+j].pop_back();
			}
		}
	}
};

void foo6()
{
	int show0=0;
	int cannalYUV=2;
	int step=1 ;
	alef00 al_;
	CvMat SUBMATR1,SUBMATR2,SUBMATR3,SUBMATR4,SUBMATR5,SUBMATR6,SUBMATR7,SUBMATR8,buf_1,buf_2,MED_MATR,MED_MATR_U,gray_mat,gray_mat_r,matr_, SUBMATR1r,SUBMATR2r;
	int sum_samples=max_len;
	CvMat* MM= cvCreateMat( 3000, 2000*(sum_samples+1), CV_8UC1 );
	cvZero(MM);
	int x_,y_;

	// It was earlier
	//MED_MATR=al_.Create_Samples_and_Mediane_Im_fromAVI_(MM,path,x_,y_,step,(int)(sum_samples/step));
	//MED_MATR_U=al_.Create_Samples_and_Mediane_Im_from_AVI_can_YUV(MM,path,cannalYUV,x_,y_,step,(int)(sum_samples/step));
	if(show0) {
		//al_.visu_float(&MED_MATR,string("mediana"),1);
		//al_.visu_float(&MED_MATR_U,string("mediana U"),7);
	}
	
	float med;
	int x_0=50;
	int y_0=50;
	float *buff=new float[x_0*y_0*2];
	//CString s,
		  
	CvMat *Diff=cvCreateMat(1,x_0*y_0-1 ,CV_32FC1);
	cvZero(Diff);
	//path=string("C:\\STORE\\XX\\FromHQ\\40_Black(FireWork)_29.97p_1.avi");
    //path_out=string("C:\\STORE\\XX\\FromHQ\\out_00");
	 
	 CreateDirectory( path_out.c_str()   ,NULL);	
	//////////////////////
	  
	if(show0){
		cvNamedWindow( "filmik",1);   // create the image window 
		cvNamedWindow( "resize",2);   // create the image window 
		cvNamedWindow( "grr",6);   // create the image window 
		cvNamedWindow( "mask_",0); 
		cvNamedWindow( "mask_morph_",0);
		cvNamedWindow( "yuv",2);
		cvNamedWindow( "u",2);
	}
	int jk;
	
	// Structure for getting video from camera or avi
    bool first=true;
    
	CvCapture* capture = 0;
    capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *frame, *frame_copy= 0;
	IplImage gray_img_hdr,gray_img_hdr1,gray_img_hdr_r, *gray_img, *gray_img_r;
	IplImage *field_img_=0;
	IplImage *part_img=0;
	IplImage *part_img_r=0;
    CvMat field_mat,*small_,*small_f;
    double tr_comp;
	IplConvKernel* element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_ELLIPSE );
    CvMat  *YUV_;
	IplImage *img_yuv;
    CvMat *field_mat_, *field_mat_morph_,*field_mat_buff_; 
    int qi,qj;
	
	//<< new code
	int medSize = 5;
	queue<IplImage*> frames;
	
	IplImage *frameBuffer;
		
	mediana localMediana(medSize);
	for (int i = 0; i < medSize+1; ++i){
		frameBuffer = cvQueryFrame(capture);
		IplImage* temp = cvCreateImage(cvSize(frameBuffer->width,frameBuffer->height), IPL_DEPTH_8U, frameBuffer->nChannels);
		cvCopy(frameBuffer, temp, 0);
		
		frames.push(temp);
		localMediana.addNext(temp);
	}
	
	int CurFrameID = 0;
	while (!frames.empty()) {
		int t = CurFrameID + 1;
		frame = frames.front();
	//>> new code
	//}
	//for(int t=1;t<max_len;t++){
		//cvGrabFrame( capture );
		//frame = cvRetrieveFrame( capture );         
		if( frame->origin == IPL_ORIGIN_TL ){
			if(first){
				gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				img_yuv=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 3);
				gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				small_f=cvCreateMat(x_0,y_0,CV_32FC1);
				small_=cvCreateMat(x_0,y_0,CV_8UC1);
				frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
				cvCopy( frame, frame_copy, 0 );
				field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
				x_=gray_mat_r.cols;
				y_=gray_mat_r.rows;
				field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
				field_mat_morph_ =cvCreateMat( y_, x_,CV_8UC1 );
				field_mat_buff_ =cvCreateMat( y_, x_,CV_8UC1 );
				YUV_=cvCreateMat( y_, x_,CV_8UC1 );
				first=false;
			}
	    
			al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
			cvCvtColor(frame, img_yuv, CV_BGR2YCrCb);

			al_.GetMatNeededCanFrom3DIm(img_yuv,  YUV_,cannalYUV);
			if(show0){
				cvShowImage("yuv",  img_yuv);
				cvShowImage("u",  YUV_);
			}
       		
			cvReshape( field_img_, &field_mat, 1 ); //create matrix	
			


			
			lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
			lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
			cvResize(&gray_mat,small_,CV_INTER_LINEAR);
			cvConvert(small_,small_f);
			
			if(show0)
			{
			cvShowImage("resize", small_f);  // display it
			cvShowImage("filmik", frame);  // display it
			cvShowImage("grr", gray_img);  // display other
			}
			float *data_=(float*)(small_f->data.i );

			//al_.visu_array (data_,2500,6,string("array"));
			vector<float> fv( data_, data_+small_->cols*small_->rows);
			sort(fv.begin(), fv.end());
			med=Mediana( fv);
			float *pfv=&(fv.at(0));
          
			int b_=(fv.size()-1);
			float sigma=50.4;
			float power_= 3;
			for ( jk=0;jk< b_+1;jk++){
				*(buff+jk)= 1-exp(- pow(  abs(*(pfv+jk)- med )  ,power_) /pow( sigma   ,power_));
           
			}
			// al_.visu_array2 (buff,x_0*y_0,4,string("array"),10 );
		 
			for ( jk=0;jk< b_;jk++){
				CV_MAT_ELEM(  *Diff, float, 0,jk)=fabs(*(buff+jk)-*(buff+jk+1));
			}

			double min_,max_;
			CvPoint LocMin_,LocMax_;
			cvMinMaxLoc(Diff,&min_,&max_,&LocMin_,&LocMax_);
			if(show0){
 				al_.visu_vector(Diff,4,string("diff"),1,300  ,2500);
				al_.visu_array2 (buff,x_0*y_0,4,string("buff"),100 );
				al_.visu_array2 (pfv,x_0*y_0,8,string("pfv"),0.8 ); 
			}
			int index_=LocMax_.x;
			float thr_0=abs(*(pfv+index_+1)- med);
			
			//<< new code
			MED_MATR = localMediana.get(CurFrameID);
			//>> new code
			
			cvSet( field_mat_,cvScalar(1)); 
			for(int qi= 0; qi<y_ ;qi++){
				for(int pi= 0;pi<x_ ;pi++){
					if(
						(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-(float)CV_MAT_ELEM(  MED_MATR, BYTE, qi, pi))>thr_ )
						||
						(true)
						//(abs((float)CV_MAT_ELEM(  *YUV_, BYTE, qi, pi)-(float)CV_MAT_ELEM(  MED_MATR_U, BYTE, qi, pi))>1.5*thr_ )
					)
					{
						CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=0;

						//*(three_channel_mat.data.ptr  + (size_t)three_channel_mat.step * qi + 3 * pi + 0) 
						//*(three_channel_mat.data.ptr  + (size_t)three_channel_mat.step * qi + 3 * pi + 1)
						//*(three_channel_mat.data.ptr  + (size_t)three_channel_mat.step * qi + 3 * pi + 2)

					}
				}
			}
			
			//<< new code
			cvReleaseImage(&frame);
			frames.pop(); // delete first element
					
			// add next image
			if (CurFrameID + frames.size() < max_len){
				frameBuffer = cvQueryFrame(capture);
				IplImage* temp = cvCreateImage(cvSize(frameBuffer->width,frameBuffer->height), IPL_DEPTH_8U, frameBuffer->nChannels);
				cvCopy(frameBuffer, temp, 0);
				
				frames.push(temp);
				localMediana.addNext(temp);
			} else {
				localMediana.addNext();
			}
					
			++CurFrameID;
			//>> new code

	///////////////
	cvMorphologyEx(
	field_mat_,
	field_mat_morph_,
	field_mat_buff_,
	element,
	CV_MOP_OPEN );
	//CV_MOP_CLOSE );
	  

    float thr_ = 0.5;
    bool show_  = 0;
    
	CvMat* FG0 = inpaint_milk_sauf02(field_mat_morph_, gray_img, thr_, show_);
	cvSet( field_mat_buff_,cvScalar(1)); 

	cvSub(field_mat_buff_,FG0,field_mat_morph_);
	cvReleaseMat( &FG0);
    cvScale(field_mat_morph_,field_mat_ , 255);
	ConnectedComponents* CC = new ConnectedComponents(field_mat_morph_);
    //CvMat* result = CC->drawComponents(20,false  ); 
	 CvMat* morph_delete_small_ = CC->drawComponents(5,true  ); 
	  if(show0)
	  {
   cvNamedWindow("6531");cvShowImage("6531", field_mat_);
   cvNamedWindow("6534");cvShowImage("6534", morph_delete_small_);
	 cvWaitKey(0);
	  }
       
	   //////////////
 
 cvSaveImage(lean_mask.data(),morph_delete_small_);
cvReleaseMat(&morph_delete_small_);
delete CC;
//cvMorphologyEx(
//field_mat_morph_,
//field_mat_,
//field_mat_buff_,
//element,
//CV_MOP_OPEN );
////CV_MOP_CLOSE );
    
    
     cvSaveImage(lean.data(),&gray_mat);
     
if(show0) cvWaitKey(0);
      

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)

 localMediana.destroy(); 
 
 cvReleaseImage( &gray_img);
	    cvReleaseImage( &frame);

   	 

delete [] buff;
//getchar();
}

////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
	char fnIni[_MAX_PATH+1];
	GetFullPathNameA( argv[1]?argv[1]:"param.ini", _MAX_PATH, fnIni, NULL );
	std::clog << "Process " << fnIni << std::endl ;
	IniPar ini( fnIni );
	path=ini.RootDir;
    path_out=ini.NewDir;
	max_len=ini.MaxFrames;
	routine_=ini.routine_;
	thr_=atof(ini.thr_.data());
	//medSize = ini.medSize;
	if(routine_==4)foo4();
    if(routine_==1)foo1();
	if(routine_==5)foo5();
	if(routine_==2)foo2();
	if(routine_==6)foo6();

	return 0;
}	


