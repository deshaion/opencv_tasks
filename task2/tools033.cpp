
#include "tools033.h"

struct point{
	int x;
	int y;
};

vector<point> processCell(CvMat *MAP, int x, int y, IplImage *F, float thr_, CvMat *isProcessed);
void putPoint(map<string, point> *points, int x, int y);
void putPointV(vector<point> *points, int x, int y);

////////////////////////////////////////////////////////////
// SECONDARY FUNCTIONS

string intToStr(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

vector<point> getPointsV(CvMat *mask){
	vector<point> result;
	
	for( int x = 0; x < mask->rows; x++ ) {
		uchar* ptr = (uchar*) (mask->data.ptr + x * mask->step);

		for( int y = 0; y < mask->cols; y++) {
			if (ptr[y] == 1) {
				putPointV(&result, x, y);
			}
		}
	}

	return result;
}

/*CvScalar* getCellF(IplImage *F, int x, int y){
	uchar* ptr = (uchar*) (F->imageData + x * F->widthStep);
	return ptr[y];
}*/

int getCellM(CvMat *M, int x, int y){
	uchar* ptr = (uchar*) (M->data.ptr + x * M->step);
	return ptr[y];
}

bool setCellM(CvMat *M, int x, int y, int Value){
	uchar* ptr = (uchar*) (M->data.ptr + x * M->step);
	ptr[y] = Value;

	return 0;
}

void putPoint(map<string, point> *points, int y, int x){
	point p;
	p.x = x;
	p.y = y;

	(*points)["x"+intToStr(x)+"y"+intToStr(y)] = p;
}

void putPointV(vector<point> *points, int x, int y){
	point p;
	p.x = x;
	p.y = y;

	(*points).push_back(p);
}

double diffScalar(vector<int> x, vector<int> y) {
	CvMat* a = cvCreateMat(1, x.size(), CV_8UC1);
	cvZero(a);
	uchar* ptr = (uchar*) a->data.ptr;
	for (int i = 0; i < x.size(); ++i){
		ptr[i] = x[i];
	}
	
	CvMat* b = cvCreateMat(1, y.size(), CV_8UC1);
	cvZero(b);
	ptr = (uchar*) b->data.ptr;
	for (int i = 0; i < y.size(); ++i){
		ptr[i] = y[i];
	}

	double res = cvNorm(a, b, CV_L1);
	if (res < 0) return -res;

	return res;
}

vector<int> GetVectorIm(IplImage *F, int x, int y){
	vector<int> res;
	
	uchar* ptr = (uchar*) (F->imageData + x * F->widthStep);
	for (int i = 0; i < F->nChannels; ++i){
		res.push_back(ptr[y+i]);
	}
	
	return res;
}

////////////////////////////////////////////////////////////
// MAIN FUNCTIONS

CvMat* inpaint_milk_sauf03(CvMat *cluster_fore_, IplImage *FG, float thr_, bool show){
    CvMat* cluster_ = cvCloneMat(cluster_fore_);
    
	vector<point> points = getPointsV(cluster_fore_);
	CvMat* isProcessed = cvCreateMat(cluster_fore_->rows, cluster_fore_->cols, CV_8UC1);
	cvZero(isProcessed);

	vector<point>::iterator it;

	while (!points.empty())
	{
		point curPoint = points.back();
		points.pop_back();

		vector<point> PointsForProcess = processCell(cluster_, curPoint.x, curPoint.y, FG, thr_, isProcessed);

		for (it = PointsForProcess.begin(); it < PointsForProcess.end(); it++ ) {
			points.push_back(*it);
		}
	}
	
	if(show) {
		cvNamedWindow("5345");
		IplImage* imgShow = cvCreateImage(cvGetSize(FG), IPL_DEPTH_8U, 1);
		cvScaleAdd(cluster_, cvRealScalar(100), FG, imgShow);
		cvShowImage("5345", imgShow);
			
		cvWaitKey();
		
		cvReleaseImage(&imgShow);
	}

	cvDestroyWindow("5345"); // Удаляем окно
	cvReleaseMat(&isProcessed);

	return cluster_;
}

vector<point> processCell(CvMat *MAP, int x, int y, IplImage *F, float thr_, CvMat *isProcessed){
	
	vector<point> PointsForProcess;
	if (getCellM(isProcessed, x, y) == 1) return PointsForProcess;
	
	int m = MAP->rows - 1;
	int n = MAP->cols - 1;
	
	int zenter_xx1 = min(m,x+1);
	int zenter_xx2 = max(0,x-1);
	int zenter_yy1 = min(n,y+1);
	int zenter_yy2 = max(0,y-1);
		
	/*CvScalar* level = getCellF(F, x, y);*/
	vector<int> level = GetVectorIm(F, x, y);
	
	vector<point> PointsForCompare;
	putPointV(&PointsForCompare, x			, zenter_yy1);
	putPointV(&PointsForCompare, x			, zenter_yy2);
	putPointV(&PointsForCompare, zenter_xx1	, y);
	putPointV(&PointsForCompare, zenter_xx1	, zenter_yy1);
	putPointV(&PointsForCompare, zenter_xx1	, zenter_yy2);
	putPointV(&PointsForCompare, zenter_xx2	, y);
	putPointV(&PointsForCompare, zenter_xx2	, zenter_yy1);
	putPointV(&PointsForCompare, zenter_xx2	, zenter_yy2);
	
	vector<point>::iterator it;
	
	for (it = PointsForCompare.begin(); it < PointsForCompare.end(); it++ ) {
		if ((getCellM(MAP, it->x, it->y) == 0) && (diffScalar(level, GetVectorIm(F, it->x, it->y)) < thr_) ){
			setCellM(MAP, it->x, it->y, 1); 
			putPointV(&PointsForProcess, it->x, it->y);
		}
	}

	setCellM(isProcessed, x, y, 1);
	
	return PointsForProcess;
}

