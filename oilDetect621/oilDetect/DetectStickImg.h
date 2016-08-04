#pragma once
#include "detectvalueregions.h"
class DetectStickImg :
	public DetectValueRegions
{
public:
	DetectStickImg(void);
	~DetectStickImg(void);
	std::vector<cv::Rect> destArea( const cv::Mat& testImg, cv::Mat& resutlImg) override;
private:
	static bool compareSize(std::vector<cv::Point> &first,std::vector<cv::Point> &second);
};

