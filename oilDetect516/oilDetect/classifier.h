#pragma once

#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <vector>
#include "DetectRagionsContext.h"
#include "DetectContext.h"
#include "SegmentContext.h"

class classifier
{
public:
	classifier(void);
	~classifier(void);
	typeofImg classify(const cv::Mat testImg);
private:
	bool circleOrMetal(const cv::Mat testImg);
	void detectBars( std::vector<cv::Rect> &metalBars);
	int  averPos( std::vector<int> collecHeight );
	int  averOffset( std::vector<int> collecHeight);
};

