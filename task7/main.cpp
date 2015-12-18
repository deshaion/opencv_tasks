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

void ApplyMask(CvMat* src, CvMat* pMask){
	if ((src->rows != pMask->rows) || (src->cols != pMask->cols) || (src->type != pMask->type)){
		CvMat* temp = cvCreateMat(src->rows, src->cols, src->type);
		cvResize(pMask, temp);
		cvMul(src, temp, src);

		cvReleaseMat(&temp);
	} else {
		cvMul(src, pMask, src);
	}
}

void foo1(string path, string path_out, int begin_, int max_len, float thr_, int morph_measure_, int show_, CvMat* pMask)
{
	string lean,lean_mask,lean_clust;
	float med;
	int show0=show_;
	alef00 al_;
	int x_,y_;
	 
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
	IplImage *pointerframeCapture;
	// from DOC: The returned image should not be released or modified by user.
	// !!!we cant delete, modify, release *pointerframeCapture!!!
	IplImage *frame, *frame_copy, *gray_img, *gray_img_r, *field_img_;
	CvMat *small_,*small_f;
    double tr_comp;
	CvMat gray_mat,gray_mat_r; 
	 
	CvMat* field_mat_; 
    int qi,qj;
for(int t=1;t<max_len;t++)
        {
	cvGrabFrame( capture );
	pointerframeCapture = cvRetrieveFrame( capture );
	if (first) frame = cvCreateImage(cvSize(pointerframeCapture->width, pointerframeCapture->height), IPL_DEPTH_8U, pointerframeCapture->nChannels);
	cvCopy(pointerframeCapture, frame, 0);
 
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
	if(t>begin_)
	{
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
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
		   CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=1;

	   }
	   }

	   }

   ConnectedComponents* CC = new ConnectedComponents(field_mat_);
   CvMat* morph_delete_small_ = CC->drawComponents(morph_measure_,true  ); 
   delete CC;
  
   if(show_) cvShowImage("mask", morph_delete_small_);
	 
	 
   ApplyMask(morph_delete_small_, pMask);
   
   cvSaveImage(lean.data(),&gray_mat);
     cvSaveImage(lean_mask.data(),morph_delete_small_);
	  cvReleaseMat(&morph_delete_small_);
      if(show0)   cvWaitKey(0);

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//if(t>begin_)
 }//for(int t=1;t<1000;t++)
 
	cvReleaseImage( &gray_img);
	cvReleaseImage( &frame);
	cvReleaseImage( &frame_copy);
	cvReleaseImage( &gray_img_r);
	cvReleaseImage( &field_img_);

	cvReleaseMat(&small_);
	cvReleaseMat(&small_f);

	cvReleaseMat(&field_mat_);

	cvReleaseCapture(&capture);

	getchar();
}


///////////////////////////////////////////////////////////////////

void foo2(string path, string path_out, int begin_, int max_len, float thr_, int morph_measure_, int show_, CvMat* pMask, float sigma, float power_)// adapted thresholding
{
	string lean,lean_mask;
	float med;
	 int x_0=50;
	 int y_0=50;
	 float *buff=new float[x_0*y_0*2];
	 //CString s,
	 alef00 al_;
	 int x_,y_;
	 CvMat *Diff=cvCreateMat(1,x_0*y_0-1 ,CV_32FC1);
	 cvZero(Diff);
	 
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
	IplImage *pointerframeCapture;
	// from DOC: The returned image should not be released or modified by user.
	// !!!we cant delete, modify, release *pointerframeCapture!!!
	IplImage *frame, *frame_copy= 0;
	IplImage *gray_img, *gray_img_r;
	IplImage *field_img_=0;
	CvMat *small_,*small_f;
    double tr_comp;
	CvMat gray_mat,gray_mat_r; 
	  
    CvMat* field_mat_; 
    int qi,qj;
for(int t=1;t<max_len;t++)
        {
        cvGrabFrame( capture );
	pointerframeCapture = cvRetrieveFrame( capture );
	if (first) frame = cvCreateImage(cvSize(pointerframeCapture->width, pointerframeCapture->height), IPL_DEPTH_8U, pointerframeCapture->nChannels);
	cvCopy(pointerframeCapture, frame, 0);
 
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
	if(t>begin_)
	{
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
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
		 if(show_)
		 {
 		 al_.visu_vector(Diff,4,string("diff"),1,300  ,2500);
		    al_.visu_array2 (buff,x_0*y_0,4,string("buff"),100 );
           al_.visu_array2 (pfv,x_0*y_0,8,string("pfv"),0.8 );
		 }
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

     ApplyMask(field_mat_, pMask);
   
	 cvSaveImage(lean.data(),&gray_mat);
     cvSaveImage(lean_mask.data(),field_mat_);
        cvWaitKey(0);

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//if(t>begin_)
 }//for(int t=1;t<1000;t++)
	cvReleaseImage( &frame);
	cvReleaseImage( &frame_copy);
	cvReleaseImage( &gray_img);
	cvReleaseImage( &gray_img_r);
	cvReleaseImage( &field_img_);

	cvReleaseMat(&Diff);
	cvReleaseMat(&small_);
	cvReleaseMat(&small_f);
	cvReleaseMat(&field_mat_);

	cvReleaseCapture(&capture);

	delete [] buff;
	getchar();
}

////////////////////////////////////////////////////////////////

void foo4(string path, string path_out, int begin_, int max_len, float thr_, int morph_measure_, int show_, CvMat* pMask, int cannalYUV, int step, float sigma, float power_, float thr_milk, int areaSize)// mediane via time
{
	string lean,lean_mask,lean_clust;
	int show0= show_ ;
	alef00 al_;
	CvMat MED_MATR,MED_MATR_U,gray_mat,gray_mat_r;
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

	cvReleaseMat(&MM);

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
	IplImage *pointerframeCapture;
	// from DOC: The returned image should not be released or modified by user.
	// !!!we cant delete, modify, release *pointerframeCapture!!!
	IplImage *frame, *frame_copy= 0;
	IplImage *gray_img, *gray_img_r;
	IplImage *field_img_=0;
	CvMat *small_,*small_f;
    double tr_comp;
	IplConvKernel* element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_ELLIPSE );
    CvMat  *YUV_;
	IplImage *img_yuv;
    CvMat *field_mat_, *field_mat_morph_,*field_mat_buff_; 
    int qi,qj;
	
	for(int t=1;t<max_len;t++){
		cvGrabFrame( capture );
		
		pointerframeCapture = cvRetrieveFrame(capture);
		if (first) frame = cvCreateImage(cvSize(pointerframeCapture->width, pointerframeCapture->height), IPL_DEPTH_8U, pointerframeCapture->nChannels);
		cvCopy(pointerframeCapture, frame, 0);
		
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
			if(t>begin_)
			{
	    
			al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
			cvCvtColor(frame, img_yuv, CV_BGR2YCrCb);

			al_.GetMatNeededCanFrom3DIm(img_yuv,  YUV_,cannalYUV);
			if(show0){
				cvShowImage("yuv",  img_yuv);
				cvShowImage("u",  YUV_);
			}
       		
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
			for ( jk=0;jk< b_+1;jk++){
				*(buff+jk)= 1-exp(- pow(  abs(*(pfv+jk)- med )  ,power_) /pow( sigma   ,power_));
           
			}
			
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
	  

    CvMat* FG0 = inpaint_milk_sauf02(field_mat_morph_, gray_img, thr_milk, show_);
	cvSet( field_mat_buff_,cvScalar(1)); 

	cvSub(field_mat_buff_,FG0,field_mat_morph_);
	cvReleaseMat( &FG0);
    cvScale(field_mat_morph_,field_mat_ , 255);
	ConnectedComponents* CC = new ConnectedComponents(field_mat_morph_);
    CvMat* morph_delete_small_ = CC->drawComponents(areaSize, true); 
	  if(show0)
	  {
   cvNamedWindow("6531");cvShowImage("6531", field_mat_);
   cvNamedWindow("6534");cvShowImage("6534", morph_delete_small_);
	 cvWaitKey(0);
	  }
       
	   //////////////
 
	ApplyMask(morph_delete_small_, pMask);
   
	cvSaveImage(lean_mask.data(),morph_delete_small_);
	cvReleaseMat(&morph_delete_small_);
	delete CC;
    
    cvSaveImage(lean.data(),&gray_mat);
     
	if(show0) cvWaitKey(0);
      
	  }//if(t>begin_)
	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)

	cvReleaseImage(&frame);
	cvReleaseImage(&frame_copy);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&gray_img_r);
	cvReleaseImage(&field_img_);

	cvReleaseMat(&Diff);
	cvReleaseMat(&small_);
	cvReleaseMat(&small_f);
	cvReleaseMat(&field_mat_);
	cvReleaseMat(&YUV_);
	cvReleaseMat(&field_mat_morph_);
	cvReleaseMat(&field_mat_buff_);

	cvReleaseCapture(&capture);

	delete [] buff;
	getchar();
}

////////////////////////////////////////////////////////////////

void foo5(string path, string path_out, int begin_, int max_len, float thr_, int morph_measure_, int show_, CvMat* pMask)// mediane local
{
	string lean,lean_mask;
	float med;
	int show0= show_ ;
	alef00 al_;
	int x_,y_;
	 
	CreateDirectory(path_out.c_str(), NULL);	
	//////////////////////
	if(show0) {
		cvNamedWindow("filmik"	, 1);   // create the image window 
		cvNamedWindow("resize"	, 2);   // create the image window 
		cvNamedWindow("mask"	, 2);   // create the image window 
		cvNamedWindow("grr"		, 6);   // create the image window
	}
   
	// Structure for getting video from camera or avi
    bool first=true;
 
	CvCapture* capture = 0;
    capture = cvCaptureFromAVI( path.data() );         // load the avi
	IplImage *pointerframeCapture;
	// from DOC: The returned image should not be released or modified by user.
	// !!!we cant delete, modify, release *pointerframeCapture!!!
	
	IplImage *frame, *frame_copy, *gray_img, *gray_img_r;
	CvMat *small_,*small_f;
    double tr_comp;
	CvMat gray_mat,gray_mat_r; 
	 
	CvMat *field_mat_,*field_mat_buff_; 
    int qi,qj;
	for(int t=1;t<max_len;t++){
		cvGrabFrame( capture );
		
		pointerframeCapture = cvRetrieveFrame( capture );
		if (first) frame = cvCreateImage(cvSize(pointerframeCapture->width, pointerframeCapture->height), IPL_DEPTH_8U, pointerframeCapture->nChannels);
		cvCopy(pointerframeCapture, frame, 0);
		
		if( frame->origin == IPL_ORIGIN_TL ){
			if(first){
				gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				small_f=cvCreateMat(50,50,CV_32FC1);
				small_=cvCreateMat(50,50,CV_8UC1);
				frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
				cvCopy( frame, frame_copy, 0 );
				al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
				x_=gray_mat_r.cols;
				y_=gray_mat_r.rows;
				field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
				field_mat_buff_ =cvCreateMat( y_, x_,CV_8UC1 );
		  
				first=false;
			}
			if( t>begin_){
				al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
       			cvShowImage("filmik", frame);  // display it
				cvShowImage("grr", gray_img);  // display other
				lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
				lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
				cvSmooth( &gray_mat, field_mat_buff_, CV_MEDIAN ,21 );
				if(show0){
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
				for(int qi= 0; qi<y_ ;qi++){
					for(int pi= 0;pi<x_ ;pi++){
						if(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-(float)CV_MAT_ELEM(  *field_mat_buff_, BYTE, qi, pi))>thr_){
							CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=255;
						}
					}
				}

				imfill(field_mat_);
				ApplyMask(field_mat_, pMask);
   
				cvSaveImage(lean.data(),&gray_mat);
				cvSaveImage(lean_mask.data(),field_mat_);
		
				if(show0){
					cvShowImage("mask", field_mat_);
					cvWaitKey(0);
				}
			}// if( frame->origin == IPL_ORIGIN_TL )
		}//if( t>begin_)
	}//for(int t=1;t<1000;t++)
 
	cvReleaseImage(&frame);
	cvReleaseImage(&frame_copy);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&gray_img_r);
	
	cvReleaseMat(&small_);
	cvReleaseMat(&small_f);
	cvReleaseMat(&field_mat_);
	cvReleaseMat(&field_mat_buff_);
	
	cvReleaseCapture(&capture);

	getchar();
}

////////////////////////////////////////////////////////////////

void foo6(string path, string path_out, int max_len, float thr_, int show_, CvMat* pMask, int medSize, int cannalYUV)
{
	string lean,lean_mask,lean_clust;
	int show0=show_;
	alef00 al_;
	CvMat MED_MATR,gray_mat,gray_mat_r;
	int sum_samples=max_len;
	int x_,y_;
	
	float med;
	int x_0=50;
	int y_0=50;
		  
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
	IplImage *gray_img, *gray_img_r;
	double tr_comp;
	CvMat  *YUV_;
	IplImage *img_yuv;
    CvMat *field_mat_;
    int qi,qj;
	string lean1;
	//<< new code
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
	    
		if( frame->origin == IPL_ORIGIN_TL ){
			if(first){
				gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				img_yuv=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 3);
				gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
				frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
				cvCopy( frame, frame_copy, 0 );
				al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
				x_=gray_mat_r.cols;
				y_=gray_mat_r.rows;
				field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
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
       		
			lean1=path_out+"\\MED_"+toString(t+1000)+".jpg";
			lean=path_out+"\\BW_"+toString(t+1000)+".jpg";
			lean_mask=path_out+"\\mask_threshold_"+toString(t+1000)+".jpg";
			
			//<< new code
			MED_MATR = localMediana.get(CurFrameID);
			 if(show0)
	  {
		   cvShowImage("grr", gray_img);  // display other
   cvNamedWindow("1531");cvShowImage("1531", &MED_MATR);
 
	 
	  }
			//>> new code
			

			cvSet( field_mat_,cvScalar(0)); 
			for(int qi= 0; qi<y_ ;qi++){
				for(int pi= 0;pi<x_ ;pi++){
					if 
						(abs((float)CV_MAT_ELEM(  gray_mat, BYTE, qi, pi)-(float)CV_MAT_ELEM(  MED_MATR, BYTE, qi, pi))>thr_ )
						 
					 
					{
						CV_MAT_ELEM( *field_mat_, BYTE, qi, pi)=255;

						 
					}
				}
			}
			
			//<< new code
			cvReleaseImage(&frame);
			frames.pop(); // delete first element
					
			// add next image
			if (CurFrameID + frames.size() < max_len)
			{
				frameBuffer = cvQueryFrame(capture);
				IplImage* temp = cvCreateImage(cvSize(frameBuffer->width,frameBuffer->height), IPL_DEPTH_8U, frameBuffer->nChannels);
				cvCopy(frameBuffer, temp, 0);
				
				frames.push(temp);
				localMediana.addNext(temp);
			} 
			else {
				localMediana.addNext();
			}
					
			++CurFrameID;
			//>> new code

	 
	   if(show0)
	   { 
   cvNamedWindow("6531");cvShowImage("6531", field_mat_);
   
	   
	   }
       
	   //////////////
 
	ApplyMask(field_mat_, pMask);
   
	cvSaveImage(lean_mask.data(),field_mat_);
 
    
     cvSaveImage(lean1.data(),&MED_MATR);
     cvSaveImage(lean.data(),&gray_mat);
     
if(show0) cvWaitKey(0);
      

	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//for(int t=1;t<1000;t++)

 localMediana.destroy(); 
 
	cvReleaseImage(&frame_copy);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&gray_img_r);
	cvReleaseImage(&img_yuv);
	
	cvReleaseMat(&field_mat_);
	cvReleaseMat(&YUV_);

	cvReleaseCapture(&capture);

	printf("the end... press enter...");getchar();
}


////////////////////////////////////////////////////////////////

void foo7(string path, string path_out, int begin_, int max_len, float thr_, int morph_measure_, int show_, CvMat* pMask, int minSizeBlob, int maxSizeBlob)
{
	string lean,lean_mask,lean_clust;
	 float med;
	 int show0=show_;
	 alef00 al_;
	 int x_,y_;
	 
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
	IplImage *pointerframeCapture;
	// from DOC: The returned image should not be released or modified by user.
	// !!!we cant delete, modify, release *pointerframeCapture!!!

	IplImage *frame, *frame_copy= 0;
	 IplImage *gray_img, *gray_img_r;
	 IplImage *field_img_=0;
	 IplImage *img_yuv;
     CvMat field_mat;
     double tr_comp;
	 CvMat gray_mat,gray_mat_r; 
	 
	 CvMat* field_mat_; 
	 CvMat  *YUV_, *YUV_1;
     int qi,qj;
for(int t=1;t<max_len;t++)
        {
        cvGrabFrame( capture );
		pointerframeCapture = cvRetrieveFrame( capture );
		if (first) frame = cvCreateImage(cvSize(pointerframeCapture->width, pointerframeCapture->height), IPL_DEPTH_8U, pointerframeCapture->nChannels);
		cvCopy(pointerframeCapture, frame, 0);
	
	if( frame->origin == IPL_ORIGIN_TL )
	  {
	if(first)
		{
         gray_img = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
         gray_img_r = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
		 img_yuv=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 3);
         frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );
         cvCopy( frame, frame_copy, 0 );
		 field_img_=cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, 1);
		  al_.GetMat1CanFromIm(frame_copy,gray_img_r, &gray_mat_r);
		  x_=gray_mat_r.cols;
          y_=gray_mat_r.rows;
		  field_mat_ =cvCreateMat( y_, x_,CV_8UC1 );
		  YUV_=cvCreateMat( y_, x_,CV_8UC1 );
		  YUV_1=cvCreateMat( y_, x_,CV_8UC1 );
         first=false;
		}
	if(t>begin_)
	{
		lean_mask=path_out+"\\mask_shape_"+toString(t+1000)+".jpg";
		lean_clust=path_out+"\\mask_cluster_"+toString(t+1000)+".jpg";
	    al_.GetMat1CanFromIm(frame,gray_img, &gray_mat);
       	cvReshape( field_img_, &field_mat, 1 ); //create matrix	
		cvCvtColor(frame, img_yuv, CV_BGR2YCrCb);
		al_.GetMatNeededCanFrom3DIm(img_yuv,  YUV_,2);
		al_.GetMatNeededCanFrom3DIm(img_yuv,  YUV_1,3);
		if(show0) 
		{ 
		cvShowImage( "resize",img_yuv);   // create the image window 
      
        cvShowImage("grr", gray_img);  // display other
       	}
       IplImage* F2 = cvCreateImage(cvGetSize(gray_img), gray_img->depth, 7);
	   cvZero(F2);
	   for( int x = 0; x < F2->height; x++) {
		uchar* psrc = (uchar*) (gray_img->imageData + x * gray_img->widthStep);
        uchar* psrc1 = ( uchar*) (YUV_->data.ptr  + x * YUV_->width  );
		 uchar* psrc2 = ( uchar*) (YUV_1->data.ptr  + x * YUV_1->width );
		uchar* pdst = (uchar*) (F2->imageData + x * F2->widthStep);
		
		for( int y = 0; y < F2->width; y++ ) {
			pdst[F2->nChannels*y+0] = psrc[y];
			pdst[F2->nChannels*y+1] = 100;
			pdst[F2->nChannels*y+2] = 150;
			pdst[F2->nChannels*y+3] = psrc1[y];
			pdst[F2->nChannels*y+4] = 150;
			pdst[F2->nChannels*y+5] = psrc2[y];
			pdst[F2->nChannels*y+6] = 59;
		}
	}
    // <<connected components of clasters
	vector<int> clusterColors;
	clusterColors.push_back(100);
	clusterColors.push_back(150);
	clusterColors.push_back(200);
	clusterColors.push_back(250);

	vector<ConnectedComponents*> ClusterConnectedComponents;

	CvMat* FG0 = Clasters(F2, 4, clusterColors);
	cvReleaseImage(&F2);
	if(show_) {
		cvNamedWindow("6534");
		cvShowImage("6534", FG0);
	}
	
	for (int i = 0; i < clusterColors.size(); ++i){
		ClusterConnectedComponents.push_back(new ConnectedComponents(FG0, clusterColors[i], clusterColors));
	}

	CvMat* POLAR  = cvCreateMat(250, 250, CV_8UC1);
	CvMat* feat_v = cvCreateMat(1, POLAR->rows, CV_32FC1);
    CvMat* BoundBox = cvCreateMat(1000, 1000, CV_8UC1); 
	 CvMat* buff = cvCreateMat(1000, 1000, CV_8UC1); 
	CvPoint centers;

	CvMat* circle_v = cvCreateMat(1, POLAR->rows, CV_32FC1 );
	CvMat* diff_v = cvCreateMat(1, POLAR->rows, CV_32FC1 );
	cvZero(circle_v);
	cvFloodFill(circle_v, cvPoint(0,0), cvScalar(1), cvScalar(0), cvScalar(0));
	float n_ = cvNorm(circle_v, NULL, CV_L2);
	cvScale(circle_v, circle_v, 1/n_);

	 //al_.visu_string_float(circle_v, 2, "circle_v", 2);

	// now bypassing the connected components
	   int sizeBlob,i,j,  iii,x_,y_,w_0,v_0 ;
	  
	   cvZero(BoundBox);
	    int xmin , xmax  ;
	    int ymin  , ymax ;
        float n1_,al_0,al_1;
	
       CvMat  SUBMATR1,SUBMATR2,SUBMATR3,SUBMATR4;
	   cvZero( field_mat_); 
	for (  i = 0; i < ClusterConnectedComponents.size(); ++i)
	{
		for (  j = 0; j < ClusterConnectedComponents[i]->blobs.size(); ++j)
		{

			 sizeBlob = ClusterConnectedComponents[i]->blobs[j].size();



			if (sizeBlob > minSizeBlob)  
			{
	xmin = 100000, xmax = 0;
   ymin = 100000, ymax = 0;

    for (int iii = 0; iii < ClusterConnectedComponents[i]->blobs[j].size(); ++iii){
		if (ClusterConnectedComponents[i]->blobs[j][iii].x < xmin) xmin = ClusterConnectedComponents[i]->blobs[j][iii].x;
		if (ClusterConnectedComponents[i]->blobs[j][iii].x > xmax) xmax = ClusterConnectedComponents[i]->blobs[j][iii].x;
		if (ClusterConnectedComponents[i]->blobs[j][iii].y < ymin) ymin = ClusterConnectedComponents[i]->blobs[j][iii].y;
		if (ClusterConnectedComponents[i]->blobs[j][iii].y > ymax) ymax = ClusterConnectedComponents[i]->blobs[j][iii].y;
	}
  w_0=ymax - ymin+1;
  v_0=xmax-xmin+1;
  cvGetCols(BoundBox,&SUBMATR1,0,w_0);
  cvGetRows( &SUBMATR1,&SUBMATR2,0,v_0);
  al_0=SUBMATR2.cols*SUBMATR2.rows ;
  al_1=al_0/1000;
  cvGetCols(buff,&SUBMATR3,0,round(w_0/al_1) );
  cvGetRows( &SUBMATR3,&SUBMATR4,0,round(v_0/al_1));
 
  cvZero(&SUBMATR2);
   for ( iii = 0; iii < ClusterConnectedComponents[i]->blobs[j].size(); ++iii)
   {
	   y_=ClusterConnectedComponents[i]->blobs[j][iii].y - ymin;
       x_=ClusterConnectedComponents[i]->blobs[j][iii].x - xmin;
	   CV_MAT_ELEM( SUBMATR2, BYTE,x_,y_ )=255;

 

		//setCellM(t, ClusterConnectedComponents[i]->blobs[j][iii].x - xmin, ClusterConnectedComponents[i]->blobs[j][iii].y - ymin, 255);
   }

   cvResize(&SUBMATR2,&SUBMATR4);


			 
 
						if (al_.polar_feature( &SUBMATR4 , centers, POLAR, feat_v))
						{
							
                            cvSub(feat_v,circle_v,diff_v);
							  n1_ = cvNorm(diff_v, NULL, CV_L2);
										 if( n1_<0.11)
										 { 
													for ( iii = 0; iii < ClusterConnectedComponents[i]->blobs[j].size(); ++iii)
													{
													y_=ClusterConnectedComponents[i]->blobs[j][iii].y  ;
													x_=ClusterConnectedComponents[i]->blobs[j][iii].x  ;
													CV_MAT_ELEM( *field_mat_, BYTE,x_,y_ )=255;
 													}
													if(show_) 
											 {
											 	cvNamedWindow(toString(n1_).data());
											    cvShowImage(toString(n1_).data(), POLAR);
											    cvNamedWindow("mask");cvShowImage("mask", field_mat_); 
												
												
											  cvNamedWindow("segm");
											    cvShowImage("segm", &SUBMATR2);
												al_.visu_string_float(feat_v, 2, "feat_v", 2);
													cvDestroyWindow(toString(n1_).data());
													cvDestroyWindow("segm");
											 }

													 
		
	  
										 }//if( n1_<1)


						}//if (al_.polar_feature( &SUBMATR2 , centers, POLAR, feat_v))
		
		
		      
			}	// if (sizeBlob > minSizeBlob)

			
		}//	for (  j = 0; j < ClusterConnectedComponents[i]->blobs.size(); ++j)
	}//for (  i = 0; i < ClusterConnectedComponents.size(); ++i)
	// >>connected components of clasters
 	
	cvReleaseMat(&POLAR);
	cvReleaseMat(&feat_v);
	cvReleaseMat(&BoundBox);
	cvReleaseMat(&buff);
	cvReleaseMat(&circle_v);
	cvReleaseMat(&diff_v);
	     
      if(show0)   cvWaitKey(0);
 
	ApplyMask(field_mat_, pMask);
   
	cvSaveImage(lean_mask.data(),field_mat_);
 	cvSaveImage(lean_clust.data(),FG0);
		cvReleaseMat(&FG0);
	  }// if( frame->origin == IPL_ORIGIN_TL )
 }//if(t>begin_)
 }//for(int t=1;t<1000;t++)
 
	cvReleaseImage(&frame);
	cvReleaseImage(&frame_copy);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&gray_img_r);
	cvReleaseImage(&field_img_);
	cvReleaseImage(&img_yuv);

	cvReleaseMat(&YUV_);
	cvReleaseMat(&YUV_1);  	 
	cvReleaseMat(&field_mat_);

	cvReleaseCapture(&capture);

	printf("the end... press enter...");getchar();
}


////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
	char fnIni[_MAX_PATH+1];
	GetFullPathNameA( argv[1]?argv[1]:"param.ini", _MAX_PATH, fnIni, NULL );
	std::clog << "Process " << fnIni << std::endl ;
	IniPar ini( fnIni );
	string path=ini.RootDir;
    string path_out=ini.NewDir;
	int max_len=ini.MaxFrames;
	int begin_=ini.begin_;
	int routine_=ini.routine_;
	int show_=ini.show_;
	int morph_measure_=ini.MorphMeasure;
	int thr_=atof(ini.thr_.data());

	// make mask
	IplImage* img = cvLoadImage("mask.jpg");
	IplImage* imgMask = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCvtColor(img, imgMask, CV_BGR2GRAY); // 3 cannal -> 1 cannal
	cvReleaseImage(&img);
	
	CvMat* mask = cvCreateMat(imgMask->height, imgMask->width, CV_8UC1);
	int _trsh = 128;
	for( int x = 0; x < imgMask->height; x++ ) {
		for( int y = 0; y < imgMask->width; y++ ) {
			setCellM(mask, x, y, (getCellF(imgMask, x, y) < _trsh));
		}
	}
	cvReleaseImage(&imgMask);
	
	if(routine_ == 1) foo1(path, path_out, begin_, max_len, thr_, morph_measure_, show_, mask);
	if(routine_ == 2) foo2(path, path_out, begin_, max_len, thr_, morph_measure_, show_, mask, 50.4, 3);
    if(routine_ == 4) foo4(path, path_out, begin_, max_len, thr_, morph_measure_, show_, mask, 2, 1, 50.4, 3, 0.5, 5);
	if(routine_ == 5) foo5(path, path_out, begin_, max_len, thr_, morph_measure_, show_, mask);
	if(routine_ == 6) foo6(path, path_out, max_len, thr_, show_, mask, 5, 2);
	if(routine_ == 7) foo7(path, path_out, begin_, max_len, thr_, morph_measure_, show_, mask, 20, 20000);

	cvReleaseMat(&mask);
	
	return 0;
}	


