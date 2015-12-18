
#include "connectedcomponents.h"
ConnectedComponents::ConnectedComponents( )
{ 
	blobs.clear();
	rows = 0;
	cols = 0;
}
ConnectedComponents::ConnectedComponents(CvMat* in)
{     
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
            cvFloodFill(label_image, cvPoint(x,y), cvScalar(label_count), cvScalar( 0), cvScalar( 0), &conComp, 4);

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

// The same as ConnectedComponents(CvMat* in), but look only one color
ConnectedComponents::ConnectedComponents(CvMat* in, int Color, vector<int> ColorsNotForUse)
{     
	blobs.clear();

	set<int> setColor;
	for (int i = 0; i < ColorsNotForUse.size(); ++i) {
		setColor.insert(ColorsNotForUse[i]);
	}
	
	rows = in->rows;
	cols = in->cols;
	
	// Fill the label_image with the blobs
    // 0+	  - background
    // Color  - unlabelled foreground
    
	CvMat* label_image = cvCloneMat(in);

    int label_count = 0; // start color
	while (true) {
		label_count++;
		if (label_count > 255) label_count = 1;
		if (setColor.find(label_count) == setColor.end()) break;
	}

    for(int y=0; y < in->rows; y++) {
        for(int x=0; x < in->cols; x++) {
            if(getCellM(label_image, y, x) != Color) {
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

			while (true) {
				label_count++;
				if (label_count > 255) label_count = 1;
				if (setColor.find(label_count) == setColor.end()) break;
			}
		}
    }

	cvReleaseMat(&label_image);
}  
///////////////////////////////////////////////////////////////
void ConnectedComponents::partition00(CvMat* in)
{     
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
    cvSet( label_image,cvScalar(255)); 
	 
    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < in->rows; y++) {
        for(int x=0; x < in->cols; x++) {
            if(getCellM(label_image, y, x) == 0) 
			{
                continue;
            }
//cvNamedWindow("634");
//	cvShowImage("634", label_image);
//	cvNamedWindow("635");
//	cvShowImage("635", in);
//	cvWaitKey(0);
	 
            CvConnectedComp conComp;
            cvFloodFill(in, cvPoint(x,y), cvScalar(label_count), cvScalar( 2), cvScalar( 2), &conComp, 4);

            vector<point> blob;
			
			CvRect rect = conComp.rect;

            for(int i=rect.y; i < (rect.y+rect.height); i++)
			{
                for(int j=rect.x; j < (rect.x+rect.width); j++) 
				{
                    if(getCellM(label_image, i, j) != label_count) 
					{
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

///////////////////////////////////////////////////////////////
ConnectedComponents::~ConnectedComponents(){     
	blobs.clear();
}  
CvMat* ConnectedComponents::drawComponents(int area, bool more) {
	CvMat* result = cvCreateMat(rows, cols, CV_8UC1);
	cvZero(result);

	for (int i = 0; i < blobs.size(); ++i){
		if (more && area < blobs[i].size()){
			drawComponent(result, i,255);
		}
		if (!more && area > blobs[i].size()){
			drawComponent(result, i,255);
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

void ConnectedComponents::showComponent00( ) 
{
	vector<point>::iterator it;
	CvMat* result = cvCreateMat(rows, cols, CV_8UC1);
	cvZero(result);
	for (int i = 0; i < blobs.size(); ++i)
	{
		cvZero(result);
		for (it = blobs[i].begin(); it < blobs[i].end(); it++ ) 
		{
		setCellM(result, it->x, it->y, 255);
	}//for (it = blobs[idxComponent].begin(); it < blobs[idxComponent].end(); it++ )


		cvNamedWindow("634");
	cvShowImage("634", result);
	cvWaitKey(0);
	 
	}//for (int i = 0; i < blobs.size(); ++i)
cvReleaseMat(&result);
}