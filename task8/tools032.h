
#include "common.h"
#include <sstream>
#include <queue>
#include <vector>

struct point{
	int x;
	int y;
};
using namespace std;


CvMat* inpaint_milk_sauf02(CvMat* cluster_fore_, IplImage *FG, float thr_, bool show);
CvMat* inpaint_milk_sauf03(CvMat *cluster_fore_, IplImage *FG, float thr_, bool show);
int getCellF(IplImage *F, int x, int y);
int getCellM(CvMat *M, int x, int y);
bool setCellM(CvMat *M, int x, int y, int Value);
void putPoint(map<string, point> *points, int x, int y);
void putPointV(vector<point> *points, int x, int y);
int imfill(CvArr* Arr);
CvMat* Clasters(IplImage* in, int cluster_count, vector<int> clusterColors);
CvMat* getForPolar(vector<point> blob);
  int round(float a);
  void ApplyMask(CvMat* src, CvMat* pMask);
   CvMat*  inpaint_milk_sauf09(CvMat* mask1,  IplImage *img,IplImage *gray_img,float thr_milk,int routine1,int routine2,bool show_);
/////////////////////////////////////////////////////////////
class mediana{
	vector< vector<int> > data;
	vector<int> dataMed;
	IplImage* gray_img;
	int x, y, n;
	
public:  
	mediana(int medSize){
		n = medSize;
		x = 0;
		y = 0;
	}
	void destroy(){
		cvReleaseImage(& gray_img);
		dataMed.clear();
		data.clear();
	}
	CvMat get(int ID){
		CvMat* MED_MATR = cvCreateMat(x, y, CV_8UC1);
		cvZero(MED_MATR);

		for (int i = 0; i < x; ++i){
			for (int j = 0; j < y; ++j){
				vector<int> fv;
				fv.resize(data[i*y+j].size());
				copy(data[i*y+j].begin(), data[i*y+j].end(), fv.begin());
				sort(fv.begin(), fv.end());
				 
				setCellM(MED_MATR, i, j, Mediana( fv));
			}
		}

		return *MED_MATR;
	}
	void addNext(IplImage* img){
		bool first = false;
		if (x == 0) {
			x = img->height;
			first = true;
		}
		if (y == 0) {
			y = img->width;
			gray_img = cvCreateImage( cvSize(y, x),IPL_DEPTH_8U, 1);
		}
		cvCvtColor(img, gray_img, CV_BGR2GRAY); // 3 cannal -> 1 cannal
		
		for (int i = 0; i < x; ++i){
			for (int j = 0; j < y; ++j){
				int t = getCellF(gray_img, i, j);
				
				if (first) {
					vector<int> vt;
					vt.push_back(t);
					data.push_back(vt);
				} else {
					data[i*y+j].insert(data[i*y+j].begin(), t);

					if (data[i*y+j].size() > (n * 2 + 1)) data[i*y+j].pop_back();
				}
			}
		}
	}
	void addNext(){
		for (int i = 0; i < x; ++i){
			for (int j = 0; j < y; ++j){
				data[i*y+j].pop_back();
			}
		}
	}
};

///////////////////////////////////////////////////////////////////////////////


 