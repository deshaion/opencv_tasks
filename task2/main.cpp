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
#include "tools033.h"
//>>avdonin

using namespace std;

////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
	int cmm_ = 512;
	int cnn_ = 512;
	
	IplImage* N1 = cvLoadImage("demo.jpg");
	cvNamedWindow("6534");
	
	IplImage* F1 = cvCreateImage(cvSize(cnn_,cmm_), N1->depth, N1->nChannels);
	
	cvResize(N1, F1, CV_INTER_LINEAR);
	cvReleaseImage(&N1);
	 
    cvShowImage("6534", F1);
	cvWaitKey(0);
	
	CvMat* qq_mask = cvCreateMat(cmm_, cnn_, CV_8UC1);
    cvZero(qq_mask);

	for( int x = 99; x < 120; x++ ) {
		uchar* ptr = (uchar*) (qq_mask->data.ptr + x * qq_mask->step);

		for( int y = 99; y < 120; y++ ) {
			ptr[y] = 1;
		}
	}

    cvDestroyWindow("6534"); // Удаляем окно

	float thr_ = 3.5;
    bool show  = false;
    int limit_ = 20;
    
	CvMat* FG0 = inpaint_milk_sauf03(qq_mask, F1, thr_, show);

	cvNamedWindow("old");
	IplImage* imgShow1 = cvCreateImage(cvGetSize(F1), F1->depth, F1->nChannels);
	cvZero(imgShow1);
	cvCopy(F1, imgShow1, FG0);
	cvShowImage("old", imgShow1);
	
	cvWaitKey();

	cvReleaseImage(&imgShow1);
	cvDestroyWindow("old"); // Удаляем окно
		
	cvReleaseImage(&F1); // Освобождаем память из под картинки
	//getchar();

	return 0;
}	


