#pragma once
#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
class DetectValueRegions
{
public:
	DetectValueRegions(void);
	~DetectValueRegions(void);
	virtual std::vector<cv::Rect> destArea( const cv::Mat& testImg, cv::Mat& resutlImg) = 0;
};

