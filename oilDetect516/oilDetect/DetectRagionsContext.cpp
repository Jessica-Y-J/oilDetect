#include "DetectRagionsContext.h"


DetectRagionsContext::DetectRagionsContext()
	:findRegionsMethod(NULL)
{
}

DetectRagionsContext::DetectRagionsContext(typeofImg type )
	:findRegionsMethod(NULL)
{
	switch (type)
	{
	case circleImg:
		findRegionsMethod = new DetectCircleImg();
		break;
	case metalImg:
		findRegionsMethod = new DetectMetalImg();
		break;
	case stickImg:
		findRegionsMethod = new DetectStickImg();
		break;
	default:
		break;
	}
}

DetectRagionsContext::~DetectRagionsContext(void)
{
	if (findRegionsMethod != NULL)
	{
		delete findRegionsMethod;
	}
}

std::vector<cv::Rect> DetectRagionsContext::getValueRegions( const cv::Mat& testImg, cv::Mat& resutlImg )
{
	return findRegionsMethod->destArea(testImg,resutlImg);
}
