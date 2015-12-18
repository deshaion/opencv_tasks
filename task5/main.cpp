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

using namespace std;

////////////////////////////////////////////////////////////////
#define CVX_WHITE CV_RGB(0xff,0xff,0xff)

int imfill(CvArr* Arr){
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	
	IplImage* img_edge = cvCreateImage(cvGetSize(Arr), 8, 1);
	cvThreshold(Arr, img_edge, 128, 255, CV_THRESH_BINARY);

	int Nc = cvFindContours(img_edge, storage, &first_contour, sizeof(CvContour), CV_RETR_EXTERNAL);

    int n = 0;
	for(CvSeq* c = first_contour; c!=NULL; c=c->h_next ) {
		cvDrawContours(Arr, c, CVX_WHITE, CVX_WHITE, 0, -1, 8);
	}

	cvReleaseImage(&img_edge);
	cvReleaseMemStorage(&storage);

	return 0;
}

int main( int argc, char** argv )
{
	IplImage* N1 = cvLoadImage("demo.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvNamedWindow("6534");
	cvShowImage("6534", N1);
	cvWaitKey(0);
	
	imfill(N1);
	
	cvShowImage("6534", N1);
	cvWaitKey(0);
	
	cvDestroyWindow("6534");
	cvReleaseImage( &N1 );
	
	return 0;
}	

/*	IplImage* img_edge = cvCreateImage(cvGetSize(N1), 8, 1);
	IplImage* img_8uc3 = cvCreateImage(cvGetSize(N1), 8, 3);
	
	cvThreshold(N1, img_edge, 128, 255, CV_THRESH_BINARY);
	
	CvMemStorage* storage = cvCreateMemStorage();
	CvSeq* first_contour = NULL;

	int Nc = cvFindContours(img_edge, storage, &first_contour, sizeof(CvContour), CV_RETR_EXTERNAL);

    int n = 0;
	printf("Total Contours detected: %d\n", Nc);
	
	cvCvtColor(N1, img_8uc3, CV_GRAY2BGR);

	for( CvSeq* c=first_contour; c!=NULL; c=c->h_next ) {
		cvDrawContours(img_8uc3, c, CVX_White, CVX_BLUE, 0, -1, 8);
		printf("Contour #%d\n", n );
		cvShowImage("6534", img_8uc3 );
		printf("  %d elements:\n", c->total);
		for( int i=0; i<c->total; ++i ) {
			CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, c, i );
			printf(“    (%d,%d)\n”, p->x, p->y );
		}
		cvWaitKey(0);
		n++;
	}
	printf("Finished all contours.\n");
	
	cvCvtColor(N1, img_8uc3, CV_GRAY2BGR);
	cvReleaseImage( &img_8uc3 );
	cvReleaseImage( &img_edge );*/