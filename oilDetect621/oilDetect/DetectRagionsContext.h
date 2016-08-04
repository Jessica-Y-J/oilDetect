#pragma once
#include "DetectValueRegions.h"
#include "DetectCircleImg.h"
#include "DetectStickImg.h"
#include "DetectMetalImg.h"
enum  typeofImg{circleImg,metalImg,stickImg,onticImg,DialImg,errorImg};
class DetectRagionsContext
{
public:
	DetectRagionsContext(void);
	DetectRagionsContext(typeofImg type);
	~DetectRagionsContext(void);
	std::vector<cv::Rect> getValueRegions(const cv::Mat& testImg, cv::Mat& resutlImg);
private:
	DetectValueRegions *findRegionsMethod;
};

