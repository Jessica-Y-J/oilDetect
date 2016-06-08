#include "DetectContext.h"


DetectContext::DetectContext(void)
	: DC(NULL)
{
}

DetectContext::DetectContext( int type, double threshold)
	: DC(NULL)
{
	switch (type)
	{
	case 2 :
		DC = new DetectEdgeTwo(threshold);
		break;
	case 3 :
		DC = new DetectEdgeThree(threshold);
		break;
	default:break;
	}
}

DetectContext::~DetectContext(void)
{
	if (DC)
	{
		delete DC;
	}
}

void DetectContext::getEdge(const cv::Mat& inputImg,cv::Mat &outputImg)
{
	if (DC != NULL)
	{
		DC -> detectEdge(inputImg,outputImg);
	}

	return;
}
