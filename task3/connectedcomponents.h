
#include <vector>
#include "tools032.h"

// main class of connected components
class ConnectedComponents  
{  
	vector< vector<point> > blobs;
	int rows, cols;
   
public:  
    ConnectedComponents(CvMat* in);
	~ConnectedComponents();

	//Function return mat, on which draw connected components more of less (its second parameter) of area
	//If flag more=true, then draw components, which area more first paremeter area, else less
	CvMat* drawComponents(int area, bool more = true);
private:
	void drawComponent(CvMat* in, int idxComponent, int value = 1);
}; 