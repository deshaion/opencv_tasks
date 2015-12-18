
#include "tools032.h"
#define CVX_WHITE CV_RGB(0xff,0xff,0xff)
vector<point> processCell(CvMat *MAP, int x, int y, IplImage *F, float thr_, CvMat *isProcessed);

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

int getCellF(IplImage *F, int x, int y){
	uchar* ptr = (uchar*) (F->imageData + x * F->widthStep);
	return ptr[y];
}

int getCellM(CvMat *M, int x, int y){
	uchar* ptr = (uchar*) (M->data.ptr + x * M->step);
	return ptr[y];
}

bool setCellM(CvMat *M, int x, int y, int Value){
	uchar* ptr = (uchar*) (M->data.ptr + x * M->step);
	ptr[y] = Value;

	//printf("%d : %d\n", x, y);

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

////////////////////////////////////////////////////////////
// MAIN FUNCTIONS
CvMat* getForPolar(vector<point> blob)
{
	
	int xmin = 100000, xmax = 0;
	int ymin = 100000, ymax = 0;

	for (int i = 0; i < blob.size(); ++i){
		if (blob[i].x < xmin) xmin = blob[i].x;
		if (blob[i].x > xmax) xmax = blob[i].x;
		if (blob[i].y < ymin) ymin = blob[i].y;
		if (blob[i].y > ymax) ymax = blob[i].y;
	}
	
	CvMat* t = cvCreateMat(xmax - xmin, ymax - ymin, CV_8UC1);
	cvZero(t);

	for (int i = 0; i < blob.size(); ++i){
		setCellM(t, blob[i].x - xmin, blob[i].y - ymin, 255);
	}

	return t;
}
////////////////////////////////////////////////////////////
double diffScalar(vector<int> x, vector<int> y, CvMat *a, CvMat *b) {
	cvZero(a);
	uchar* ptr = (uchar*) a->data.ptr;
	for (int i = 0; i < x.size(); ++i){
		ptr[i] = x[i];
	}
	
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
		res.push_back(ptr[F->nChannels*y+i]);
	}
	
	return res;
}

 /////////////////////////////////////////////////////
vector<point> processCell3(CvMat *MAP, int x, int y, IplImage *F, float thr_, CvMat *isProcessed, CvMat *diffA, CvMat *diffB){
	
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
		if ((getCellM(MAP, it->x, it->y) == 0) && (diffScalar(level, GetVectorIm(F, it->x, it->y), diffA, diffB) < thr_) ){
			setCellM(MAP, it->x, it->y, 1); 
			putPointV(&PointsForProcess, it->x, it->y);
		}
	}

	setCellM(isProcessed, x, y, 1);
	
	return PointsForProcess;
}
////////////////////////////////////////////////////

///////////////////////////////////////////////////////
CvMat* inpaint_milk_sauf03(CvMat *cluster_fore_, IplImage *FG, float thr_, bool show)
{
    CvMat* cluster_ = cvCloneMat(cluster_fore_);
    
	vector<point> points = getPointsV(cluster_fore_);
	CvMat* isProcessed = cvCreateMat(cluster_fore_->rows, cluster_fore_->cols, CV_8UC1);
	cvZero(isProcessed);

	CvMat* diffA = cvCreateMat(1, FG->nChannels, CV_8UC1);
	CvMat* diffB = cvCreateMat(1, FG->nChannels, CV_8UC1);

	vector<point>::iterator it;

	while (!points.empty())
	{
		point curPoint = points.back();
		points.pop_back();

		vector<point> PointsForProcess = processCell3(cluster_, curPoint.x, curPoint.y, FG, thr_, isProcessed, diffA, diffB);

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
	cvReleaseMat(&diffA);
	cvReleaseMat(&diffB);
	cvReleaseMat(&isProcessed);

	return cluster_;
}
//////////////////////////////////////////////
 CvMat*  inpaint_milk_sauf09(CvMat* mask1,  IplImage *img,IplImage *gray_img,float thr_milk,int routine1,int routine2,bool show_)
{
CvMat *FG0=NULL;
if(routine1==1)
{
 FG0 = inpaint_milk_sauf02(mask1,  gray_img, thr_milk, show_);
}
if(routine1==2)
{
 FG0 = inpaint_milk_sauf03(mask1,  img, thr_milk, show_);
}
 

return  FG0 ;
 }
/////////////////////////////////////////////////

CvMat* inpaint_milk_sauf02(CvMat *cluster_fore_, IplImage *FG, float thr_, bool show){
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
 int  round(float a)
{
 if(a>0) return (int)(a+0.5); else return  (int)(a-0.5);
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
		
	int level = getCellF(F, x, y);

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
		if ((getCellM(MAP, it->x, it->y) == 0) && (abs(level - getCellF(F, it->x, it->y)) < thr_) ){
			setCellM(MAP, it->x, it->y, 1); 
			putPointV(&PointsForProcess, it->x, it->y);
		}
	}

	setCellM(isProcessed, x, y, 1);
	
	return PointsForProcess;
}
////////////////////////////////////////////////////////////////

CvMat* Clasters(IplImage* in, int cluster_count, vector<int> clusterColors)
{
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

void ApplyMask(CvMat* src, CvMat* pMask)
{
	if ((src->rows != pMask->rows) || (src->cols != pMask->cols) || (src->type != pMask->type)){
		CvMat* temp = cvCreateMat(src->rows, src->cols, src->type);
		cvResize(pMask, temp);
		cvMul(src, temp, src);

		cvReleaseMat(&temp);
	} else {
		cvMul(src, pMask, src);
	}
}