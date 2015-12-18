
//#ifndef _EiC
#include "common.h"
//#endif

struct point{
	int x;
	int y;
};

using namespace std;

CvMat* inpaint_milk_sauf02(CvMat* cluster_fore_, IplImage *FG, float thr_, int limit, bool show);

CvMat* inpaint_milk_sauf02_ivanMod(CvMat* cluster_fore_, IplImage *FG, float thr_, bool show);

void putPoint(map<string, point> *points, int x, int y);
void putPointV(vector<point> *points, int x, int y);


