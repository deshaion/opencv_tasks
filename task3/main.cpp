//
 
# include "common.h"
#include <fstream>
#include <conio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <sstream>
#include <algorithm>

//<<avdonin
#include "connectedcomponents.h"
#include "timer.h"
//>>avdonin

using namespace std;

////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
	// prepare mask
	IplImage* N1 = cvLoadImage("demo.jpg");

	int cmm_ = N1->height;
	int cnn_ = N1->width;
	
	IplImage* F1_ = cvCreateImage(cvGetSize(N1), IPL_DEPTH_8U, 1);
	cvCvtColor(N1, F1_, CV_BGR2GRAY); // 3 cannal -> 1 cannal
	cvReleaseImage(&N1);

	IplImage* F1 = cvCreateImage(cvSize(cnn_,cmm_), IPL_DEPTH_8U, 1);
	
	cvResize(F1_, F1, CV_INTER_LINEAR);
	cvReleaseImage(&F1_);
	 
    CvMat* qq_mask = cvCreateMat(cmm_, cnn_, CV_8UC1);
    cvZero(qq_mask);
	
	int _trsh = 100;
	
	for( int x = 0; x < F1->height; x++ ) {
		for( int y = 0; y < F1->width; y++ ) {
			setCellM(qq_mask, x, y, (getCellF(F1, x, y) < _trsh));
		}
	}

    cvReleaseImage(&F1); // release memory

	// mask ready. get connected components
	MyTimer timer; 
	double valuet;
	timer.start();
	
	ConnectedComponents* CC = new ConnectedComponents(qq_mask);

	valuet = timer.stop();
	printf("\ntimer: %.8lf", valuet);

	cvReleaseMat(&qq_mask);

	// test functions
	cvNamedWindow("6534");
	CvMat* imgShow = cvCreateMat(cmm_, cnn_, CV_8UC1);
	
	CvMat* result = CC->drawComponents(1);  // area > 
	cvZero(imgShow);
	cvAddS(result, cvRealScalar(255), imgShow, result);
	cvShowImage("6534", imgShow);
	
	cvWaitKey(0);

	result = CC->drawComponents(1, false); // area <
	cvZero(imgShow);
	cvAddS(result, cvRealScalar(255), imgShow, result);
	cvShowImage("6534", imgShow);
	
	cvWaitKey(0);
	cvDestroyWindow("6534");

	cvReleaseMat(&result);
	
	return 0;
}	


/*cvNamedWindow("6534");
	CvMat* imgShow = cvCreateMat(cmm_, cnn_, CV_8UC1);
	cvZero(imgShow);
	cvAddS(qq_mask, cvRealScalar(255), imgShow, qq_mask);
	cvShowImage("6534", imgShow);
	cvWaitKey(0);
	cvDestroyWindow("6534");*/
	
	
//CvMat* result = cvCreateMat(cmm_, cnn_, CV_8UC1);
	
	/*cvNamedWindow("6534");
	for (int i = 0; i < 5; ++i){
		cvZero(result);
		CC->drawComponent(result, i, 255);

		cvShowImage("6534", result);
		cvWaitKey(0);
	}
	cvDestroyWindow("6534");*/

	//getchar();