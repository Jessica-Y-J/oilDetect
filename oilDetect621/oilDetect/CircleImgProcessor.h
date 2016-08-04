#pragma once
#include <vector>
#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <algorithm>
#include <utility>
#include <cmath>
#include <functional>
#include "ImgProcessor.h"
#include "ResultOfDetect.h"
#include "DetectContext.h"
#include "DetectRagionsContext.h"
#include "dllfunction.h"
#include "Util.h"

class CircleImgProcessor :
	public ImgProcessor
{
public:
	typedef std::vector<std::vector<cv::Point> > connectArea;
	CircleImgProcessor(void);
	~CircleImgProcessor(void);
	 ResultOfDetect imgProcessor(const cv::Mat inputImg) override;
private:
	void storeContainerPixels(const cv::Mat containerImg,int *Xaxis,int *Yaxis,int &EqualNum);
	std::vector<std::pair<int,double> > mergeConnection(connectArea &contour);
	//void sortConnection(std::vector<std::pair<int,double> >& info_contour);
	static bool compConnection(std::pair<int,double>& leftArea,std::pair<int,double>& rightArea);
};

