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

enum segType{otsu};

class MetalImgProcessor :
	public ImgProcessor
{
public:
	MetalImgProcessor(void);
	~MetalImgProcessor(void);
	ResultOfDetect imgProcessor(const cv::Mat inputImg) override;
};

