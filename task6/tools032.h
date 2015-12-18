
#include "common.h"

struct point{
	int x;
	int y;
};

using namespace std;

CvMat* inpaint_milk_sauf02(CvMat* cluster_fore_, IplImage *FG, float thr_, bool show);
int getCellF(IplImage *F, int x, int y);
int getCellM(CvMat *M, int x, int y);
bool setCellM(CvMat *M, int x, int y, int Value);
void putPoint(map<string, point> *points, int x, int y);
void putPointV(vector<point> *points, int x, int y);
int imfill(CvArr* Arr);


