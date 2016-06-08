#pragma once
#include "imgprocessor.h"

#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include "ResultOfDetect.h"
#include "Util.h"
#include "DetectContext.h"
#include "SegmentContext.h"
#include "DetectRagionsContext.h"

class DialImgProcessor :
	public ImgProcessor
{
public:
	DialImgProcessor(void);
	~DialImgProcessor(void);
	ResultOfDetect imgProcessor(const cv::Mat inputImg);
};

