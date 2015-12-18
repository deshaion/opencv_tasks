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

CvMat* Clasters(IplImage* in, int cluster_count, vector<int> clusterColors){
	if (clusterColors.size() == 0){
		for (int i = 0; i < cluster_count; ++i) clusterColors.push_back((i * 255 / cluster_count)%255);
	}
	
	int res_count = in->height * in->width;

	CvMat* points = cvCreateMat(res_count, 1, CV_32FC(in->nChannels));
	CvMat* clusters = cvCreateMat(res_count, 1, CV_32SC1);

	cvZero(points);
	cvZero(clusters);
	
	for (int x = 0; x < in->height; ++x){
		uchar* psrc = (uchar*) (in->imageData + x * in->widthStep);

		for( int y = 0; y < in->width; y++ ) {
			
			int xdst = x*in->width + y;
			float* pdst = (float*) (points->data.ptr + xdst * points->step);
			for (int z = 0; z < in->nChannels; ++z) {
				pdst[z] = float(psrc[in->nChannels*y+z]);
			}
		}
	}
	
	cvKMeans2(points, cluster_count, clusters, cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0));

	CvMat* result = cvCreateMat(in->height, in->width, CV_8UC1);
	cvZero(result);

	for(int i = 0; i < res_count; i++ ){
		int cluster_idx = clusters->data.i[i];
		
		int x = i / in->width;
		int y = i % in->width;
		
		uchar* ptr = (uchar*) (result->data.ptr + x * result->step);
		ptr[y] = clusterColors[cluster_idx];
	}

	cvReleaseMat( &points );
	cvReleaseMat( &clusters );
        
	return result;
}


int main( int argc, char** argv ){
	IplImage* N0 = cvLoadImage("demo.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* N1 = cvCreateImage(cvSize(500,700), N0->depth, N0->nChannels);
	
	cvResize(N0, N1, CV_INTER_LINEAR);
	cvReleaseImage(&N0);

	cvNamedWindow("6534");
	cvShowImage("6534", N1);
	cvWaitKey(0);
	
	IplImage* F2 = cvCreateImage(cvGetSize(N1), N1->depth, 7);
	cvZero(F2);
	for( int x = 0; x < F2->height; x++) {
		uchar* psrc = (uchar*) (N1->imageData + x * N1->widthStep);
		uchar* pdst = (uchar*) (F2->imageData + x * F2->widthStep);
		
		for( int y = 0; y < F2->width; y++ ) {
			pdst[F2->nChannels*y+0] = psrc[y];
			pdst[F2->nChannels*y+1] = 100;
			pdst[F2->nChannels*y+2] = 150;
			pdst[F2->nChannels*y+3] = psrc[y];
			pdst[F2->nChannels*y+4] = 150;
			pdst[F2->nChannels*y+5] = psrc[y];
			pdst[F2->nChannels*y+6] = 59;
		}
	}
	
	vector<int> clusterColors;
	CvMat* FG0 = Clasters(F2, 4, clusterColors);
	
	cvShowImage("6534", FG0);
	cvWaitKey(0);
	
	cvDestroyWindow("6534");
	cvReleaseImage(&N1);
	cvReleaseImage(&F2);
	cvReleaseMat(&FG0)
	
	return 0;
}	