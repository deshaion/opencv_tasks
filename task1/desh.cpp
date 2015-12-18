
#include "desh.h"

int iter_segment_5as(map<string, point> *points, CvMat *MAP, IplImage *F, float thr_);
vector<point> processCell(CvMat *MAP, int x, int y, IplImage *F, float thr_, CvMat *isProcessed);

string intToStr(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

map<string, point> getPoints(CvMat *mask){
	map<string, point> result;
	
	for( int x = 0; x < mask->rows; x++ ) {
		uchar* ptr = (uchar*) (mask->data.ptr + x * mask->step);

		for( int y = 0; y < mask->cols; y++) {
			if (ptr[y] == 1) {
				putPoint(&result, y, x);
			}
		}
	}

	return result;
}

vector<point> getPointsV(CvMat *mask){
	vector<point> result;
	
	for( int x = 0; x < mask->rows; x++ ) {
		uchar* ptr = (uchar*) (mask->data.ptr + x * mask->step);

		for( int y = 0; y < mask->cols; y++) {
			if (ptr[y] == 1) {
				putPointV(&result, y, x);
			}
		}
	}

	return result;
}

CvMat* inpaint_milk_sauf02(CvMat *cluster_fore_, IplImage *FG, float thr_, int limit, bool show){
    CvMat* cluster_ = cvCloneMat(cluster_fore_);
    
	IplImage* FG1 = cvCloneImage(FG);
    int esh = limit * 2;

	if(show) {cvNamedWindow("5345");}

	//points_ = mask_to_points(cluster_fore_);
	
	map<string, point> points = getPoints(cluster_fore_);

	while(esh > limit){
		esh = iter_segment_5as(&points, cluster_, FG1, thr_);
		//printf("\n%d", esh);

		if(show) {
			//ai_(100*cluster_+FG)
			IplImage* imgShow = cvCreateImage(cvGetSize(FG), IPL_DEPTH_8U, 1);
			cvScaleAdd(cluster_, cvRealScalar(100), FG, imgShow);
			cvShowImage("5345", imgShow);
			
			cvWaitKey();
			
			cvReleaseImage(&imgShow);
		}
	}

	cvDestroyWindow("5345"); // Удаляем окно

	return cluster_;
}

CvMat* inpaint_milk_sauf02_ivanMod(CvMat *cluster_fore_, IplImage *FG, float thr_, bool show){
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

	cvDestroyWindow("5345");
	cvReleaseMat(&isProcessed);

	return cluster_;
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



int iter_segment_5as(map<string, point> *points, CvMat *MAP, IplImage *F, float thr_){
	int m = MAP->rows - 1;
	int n = MAP->cols - 1;
	
	map<string, point> points_new;
	
	int esh=0;

	map<string, point>::iterator jiu;
	
	for (jiu = (*points).begin(); jiu != (*points).end(); ++jiu){
		int zenter_xx = jiu->second.x;
		int zenter_yy = jiu->second.y;
	  
		int zenter_xx1 = min(m,zenter_xx+1);
		int zenter_xx2 = max(0,zenter_xx-1);
		int zenter_yy1 = min(n,zenter_yy+1);
		int zenter_yy2 = max(0,zenter_yy-1);
		
		int level = getCellF(F, zenter_xx, zenter_yy); 

		if ((getCellM(MAP, zenter_xx1, zenter_yy) == 0) && (abs(level - getCellF(F, zenter_xx1,zenter_yy)) < thr_) ){
			esh=esh+1;
			putPoint(&points_new, zenter_xx1,zenter_yy);
			setCellM(MAP, zenter_xx1,zenter_yy, 1); 
		}
		if ((getCellM(MAP, zenter_xx, zenter_yy1) == 0) && (abs(level - getCellF(F, zenter_xx, zenter_yy1)) < thr_) ){
			esh=esh+1   ;
			putPoint(&points_new, zenter_xx,zenter_yy1);
			setCellM(MAP, zenter_xx,zenter_yy1, 1); 
		}
		if ((getCellM(MAP, zenter_xx1,zenter_yy1)==0 )&&(abs(level-getCellF(F, zenter_xx1,zenter_yy1))<thr_) ){
			esh=esh+1 ;
			putPoint(&points_new, zenter_xx1,zenter_yy1);
			setCellM(MAP, zenter_xx1,zenter_yy1, 1); 
		}
		if( (getCellM(MAP, zenter_xx2,zenter_yy)==0 )&&(abs(level-getCellF(F, zenter_xx2,zenter_yy))<thr_) ) {
			esh=esh+1 ;
			putPoint(&points_new, zenter_xx2,zenter_yy);
			setCellM(MAP, zenter_xx2,zenter_yy, 1); 
		}
		if ((getCellM(MAP, zenter_xx ,zenter_yy2)==0 )&&(abs(level-getCellF(F, zenter_xx ,zenter_yy2))<thr_) ){
			esh=esh+1 ;
			putPoint(&points_new, zenter_xx ,zenter_yy2);
			setCellM(MAP, zenter_xx ,zenter_yy2, 1); 
		}
		if ((getCellM(MAP, zenter_xx2,zenter_yy2)==0) &&(abs(level-getCellF(F, zenter_xx2,zenter_yy2))<thr_) ){
			esh=esh+1 ;
			putPoint(&points_new, zenter_xx2 ,zenter_yy2);
			setCellM(MAP, zenter_xx2,zenter_yy2, 1); 
		}
		if ((getCellM(MAP, zenter_xx1,zenter_yy2)==0) &&(abs(level-getCellF(F, zenter_xx1,zenter_yy2))<thr_) ){
			esh=esh+1 ;
			putPoint(&points_new, zenter_xx1 ,zenter_yy2);
			setCellM(MAP, zenter_xx1,zenter_yy2, 1); 
		}
		if ( (getCellM(MAP, zenter_xx2,zenter_yy1)==0) &&(abs(level-getCellF(F, zenter_xx2,zenter_yy1))<thr_) ){
			esh=esh+1 ;
			putPoint(&points_new, zenter_xx2 ,zenter_yy1);
			setCellM(MAP, zenter_xx2,zenter_yy1, 1);
		}
	}
 
	//points_new=union( points_old,points_new,'rows');
	for (jiu = points_new.begin(); jiu != points_new.end(); ++jiu){
		putPoint(points, jiu->second.x, jiu->second.y);
	}

	return esh;
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

	// после пойдем в глубину по точкам
	setCellM(isProcessed, x, y, 1);
	
	return PointsForProcess;
}