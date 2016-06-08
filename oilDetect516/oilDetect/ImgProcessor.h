#pragma once

#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <vector>
#include <utility>
#include "ResultOfDetect.h"
#include "DetectContext.h"


class ImgProcessor
{
public:
	ImgProcessor(void);
	~ImgProcessor(void);
	virtual ResultOfDetect imgProcessor(const cv::Mat inputImg) = 0;
};

