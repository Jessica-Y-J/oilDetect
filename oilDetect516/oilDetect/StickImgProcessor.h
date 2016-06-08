#pragma once
#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include "ResultOfDetect.h"
#include "Util.h"
#include "DetectContext.h"
#include "SegmentContext.h"
#include "imgprocessor.h"
#include "DetectRagionsContext.h"

class StickImgProcessor :
	public ImgProcessor
{
public:
	StickImgProcessor(void);
	~StickImgProcessor(void);
	ResultOfDetect imgProcessor(const cv::Mat inputImg) override;
};

