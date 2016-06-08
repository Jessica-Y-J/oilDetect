// version 0.23

#include "InsDetector.h"

using namespace cv;
using namespace std;

bool InsDetector::init()
{
	return ((cascadeB.load("TypeB.xml"))&&(cascadeC.load("TypeC.xml")));
}

Rect InsDetector::detect(const Mat& imgin, const int IDmode)
{
	vector<Rect> loc;
	int idx = 0;
	Size imgsize = imgin.size();
	int imgx = int(imgsize.width/2);
	double distx = 0, dist = imgx*2;
	
	switch(IDmode)
	{
	case 1:
		cascadeC.detectMultiScale(imgin, loc, 1.1, 3, 0, Size(50, 50));
		break;

	default:
	case 0:
		cascadeB.detectMultiScale(imgin, loc, 1.1, 3, 0, Size(50, 50));
	}

	if (loc.size()<=0) return(Rect(1,1,1,1)); // 无检出即跳出

	for (size_t i = 0; i < loc.size(); i++)
	{
		distx = abs(loc[i].x + loc[i].width*0.5 - imgx);
		if (distx < dist)
		{
			dist = distx;
			idx = i;
		}
		// cout<<dist<<loc[idx]<<endl; // d1
	}


	switch(IDmode)
	{
	case 1: // 圆
		//loc[idx].y = max(int(loc[idx].y - loc[idx].height*0.1),1);
		//loc[idx].height = min(int(loc[idx].height*1.2),imgsize.height-loc[idx].y);
		//loc[idx].x = max(int(loc[idx].x - loc[idx].width*0.1), 1);
		//loc[idx].width = min(int(loc[idx].width*1.2),imgsize.width-loc[idx].x);
		break;

	default:
	case 0: // 长
		loc[idx].y = max(int(loc[idx].y - loc[idx].height*0.1),1);
		loc[idx].height = min(int(loc[idx].height*1.2),imgsize.height-loc[idx].y);
		//loc[idx].x = max(int(loc[idx].x + loc[idx].width*0.05), 0);
		//loc[idx].width = int(loc[idx].width*0.9);
		break;
	}
	// cout<<(idx+1)<<" in "<<loc.size()<<endl; // d2

	return loc[idx];
}