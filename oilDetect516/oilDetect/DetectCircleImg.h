#pragma once
#include "detectvalueregions.h"
class DetectCircleImg :
	public DetectValueRegions
{
public:
	DetectCircleImg(void);
	~DetectCircleImg(void);
	std::vector<cv::Rect> destArea( const cv::Mat& testImg, cv::Mat& resutlImg) override;
};

