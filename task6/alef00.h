#pragma once
#include <fstream>
#include <conio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <sstream>
#include <algorithm>
# include "common.h"
class alef00
{
public:
	alef00(void);
	~alef00(void);
	void   GetMat1CanFromIm(IplImage *frame,IplImage *gray_img, CvMat *gray_mat);
	void  visu_vector(CvMat* vect,int n_,std::string name,int routine,float el, int end_);
	void  visu_array(float* vect,int qwe,int n_,std::string name);
	void  visu_array2(float* vect,int qwe,int n_,std::string name,float el);
	 template <class T>  void  visu_array1(T* vect,int qwe,int n_,std::string name);
	 CvMat   Create_Samples_and_Mediane_Im_fromAVI_(CvMat* MM ,std::string path_avi,int& x_,int& y_,int step,int sum_samples);
	 CvMat   Create_Samples_and_Mediane_Im_from_AVI_can_YUV(CvMat* MM ,std::string path_avi,int cannal, int& x_,int& y_,int step,int sum_samples);

	 void  visu_float(CvMat *mat,std::string s,int n_);
	 void   GetMatNeededCanFrom3DIm(IplImage *frame,  CvMat * mat,int cannal);
};
