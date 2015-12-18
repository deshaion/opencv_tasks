
#include <vector>
#include <set>
#include "tools032.h"

// main class of connected components
class ConnectedComponents  
{  
	int rows, cols;

public:	
	vector< vector<point> > blobs;
   
public:  
	ConnectedComponents(CvMat* in);
	ConnectedComponents(CvMat* in, int color, vector<int> ColorsNotForUse);
	ConnectedComponents();
	~ConnectedComponents();
    void  showComponent00();
	void  partition00(CvMat* in);
	//Function return mat, on which draw connected components more of less (its second parameter) of area
	//If flag more=true, then draw components, which area more first paremeter area, else less
	CvMat* drawComponents(int area, bool more = true);
private:
	void drawComponent(CvMat* in, int idxComponent, int value = 1);
}; 