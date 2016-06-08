#pragma once
#include "imgprocessor.h"
#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include "ResultOfDetect.h"
#include "Util.h"
#include "imgprocessor.h"

#include "DetectContext.h"
#include "DetectRagionsContext.h"


class OnticImgProcessor :
	public ImgProcessor
{
public:
	OnticImgProcessor(void);
	~OnticImgProcessor(void);
	ResultOfDetect imgProcessor(const cv::Mat inputImg) override;
private:
	void swap(cv::Rect *first,cv::Rect *second);
};

