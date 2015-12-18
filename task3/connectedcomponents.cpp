
#include "connectedcomponents.h"

ConnectedComponents::ConnectedComponents(CvMat* in){     
	blobs.clear();

	rows = in->rows;
	cols = in->cols;
	
	// Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

	//CvMat* label_image = cvCreateMat(in->rows, in->cols, CV_32FC1);
    //cvCvtColor(in, label_image, CV_GRAY2BGR);
	CvMat* label_image = cvCloneMat(in);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < in->rows; y++) {
        for(int x=0; x < in->cols; x++) {
            if(getCellM(label_image, y, x) != 1) {
                continue;
            }

            CvConnectedComp conComp;
            cvFloodFill(label_image, cvPoint(x,y), cvScalar(label_count), cvScalar(0), cvScalar(0), &conComp, 4);

            vector<point> blob;
			
			CvRect rect = conComp.rect;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(getCellM(label_image, i, j) != label_count) {
                        continue;
                    }

                    putPointV(&blob, i, j);
					setCellM(label_image, i, j, 0);
                }
            }

            blobs.push_back(blob);

            label_count++;
			if (label_count > 255) label_count = 2;
        }
    }

	cvReleaseMat(&label_image);
}  
ConnectedComponents::~ConnectedComponents(){     
	blobs.clear();
}  
CvMat* ConnectedComponents::drawComponents(int area, bool more) {
	CvMat* result = cvCreateMat(rows, cols, CV_8UC1);
	cvZero(result);

	for (int i = 0; i < blobs.size(); ++i){
		if (more && area < blobs[i].size()){
			drawComponent(result, i);
		}
		if (!more && area > blobs[i].size()){
			drawComponent(result, i);
		}
	}

	return result;
}  
 
void ConnectedComponents::drawComponent(CvMat* in, int idxComponent, int value) {
	vector<point>::iterator it;

	for (it = blobs[idxComponent].begin(); it < blobs[idxComponent].end(); it++ ) {
		setCellM(in, it->x, it->y, value);
	}
} 