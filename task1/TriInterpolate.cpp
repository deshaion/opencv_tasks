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

//<<deshaion
#include "desh.h"
#include "timer.h"
//>>deshaion

using namespace std;

////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
	int cmm_ = 1000;
	int cnn_ = 130;
	
	IplImage* N1 = cvLoadImage("demo.jpg");
	cvNamedWindow("6534");
	//cvShowImage("6534", N1);
	//cvWaitKey(0);
	
	IplImage* F1_ = cvCreateImage(cvGetSize(N1), IPL_DEPTH_8U, 1);
	cvCvtColor(N1, F1_, CV_BGR2GRAY); // 3 cannal -> 1 cannal
	cvReleaseImage(&N1);

	IplImage* F1 = cvCreateImage(cvSize(cnn_,cmm_), IPL_DEPTH_8U, 1);
	
	cvResize(F1_, F1, CV_INTER_LINEAR);
	cvReleaseImage(&F1_);
	 
    cvShowImage("6534", F1);
	cvWaitKey(0);
	
    CvMat* qq_mask = cvCreateMat(cmm_, cnn_, CV_8UC1);
    cvZero(qq_mask);

	// Получим 5 строку 250 элемент
	/*uchar* ptr = (uchar*) (qq_mask->data.ptr + 99 * qq_mask->step);
	ptr[499] = 1;

	ptr = (uchar*) (F1->imageData + 99 * F1->widthStep);
	ptr[499] = 1;*/
	
	for( int x = 99; x < 120; x++ ) {
		uchar* ptr = (uchar*) (qq_mask->data.ptr + x * qq_mask->step);

		for( int y = 99; y < 120; y++ ) {
			ptr[y] = 1;
		}
	}

    cvDestroyWindow("6534"); // Удаляем окно
	
	float thr_ = 1.5;
    bool show  = false;
    int limit_ = 20;
    
	// по старому алгоритму
	
	MyTimer timer; 
	double valuet;
	timer.start();
	
	CvMat* FG0 = inpaint_milk_sauf02(qq_mask, F1, thr_, limit_, show);

	valuet = timer.stop(); //останавливаем таймер (значение он запоминает)  
	printf("\ntimer: %.8lf", valuet);

	// по новому алгоритму
	timer.start();
	
	CvMat* FG1 = inpaint_milk_sauf02_ivanMod(qq_mask, F1, thr_, show);

	valuet = timer.stop(); //останавливаем таймер (значение он запоминает)  
	printf("\ntimer: %.8lf", valuet);
	
	cvNamedWindow("old");
	IplImage* imgShow1 = cvCreateImage(cvGetSize(F1), IPL_DEPTH_8U, 1);
	cvScaleAdd(FG0, cvRealScalar(100), F1, imgShow1);
	cvShowImage("old", imgShow1);
	
	cvNamedWindow("new");
	IplImage* imgShow2 = cvCreateImage(cvGetSize(F1), IPL_DEPTH_8U, 1);
	cvScaleAdd(FG1, cvRealScalar(100), F1, imgShow2);
	cvShowImage("new", imgShow2);
	cvWaitKey();

	int diff;
	//CvMat* res = cvCreateMat(FG0->rows, FG0->cols, CV_8UC1);
	//cvCmp(FG0, FG1, res, CV_CMP_NE);
	//diff = cvCountNonZero(res);
	//printf("\ndiff by cvCountNonZero: %d", diff);
	
	diff = 0;
	for( int x = 0; x < FG1->rows; x++ ) {
		uchar* ptr1 = (uchar*) (FG0->data.ptr + x * FG0->step);
		uchar* ptr2 = (uchar*) (FG1->data.ptr + x * FG1->step);

		for( int y = 0; y < FG1->cols; y++) {
			if (ptr1[y] != ptr1[y]) {
				//printf("\n%d %d", y, x);
				++diff;
			}
		}
	}

	printf("\ndiff: %d", diff);
	
	cvReleaseImage(&imgShow2);
	cvDestroyWindow("new"); // Удаляем окно
	
	cvReleaseImage(&imgShow1);
	cvDestroyWindow("old"); // Удаляем окно
	
	cvReleaseImage(&F1); // Освобождаем память из под картинки
	getchar();

	return 0;
}	


