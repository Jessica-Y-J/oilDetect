#pragma once
#include "detectvalueregions.h"
class DetectMetalImg :
	public DetectValueRegions
{
public:
	DetectMetalImg(void);
	~DetectMetalImg(void);
	std::vector<cv::Rect> destArea( const cv::Mat& testImg, cv::Mat& resutlImg) override;
private:
	 void detectBars( std::vector<cv::Rect> &metalBars);
	 int  averPos( std::vector<int> collecHeight );
	 int  averOffset( std::vector<int> collecHeight);
};

