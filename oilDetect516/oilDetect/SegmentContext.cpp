#include "SegmentContext.h"


SegmentContext::SegmentContext(void)
	: segmentStrategy(NULL)
{
}

SegmentContext::SegmentContext(int type)
	: segmentStrategy(NULL)
{
	enum segType{otsu};
	switch(type)
	{
	case otsu:
		segmentStrategy = new ThresholdOtsu();
		break;
	default:
		break;
	}
}

SegmentContext::~SegmentContext(void)
{
	if (segmentStrategy)
	{
		delete segmentStrategy;
	}
}

void SegmentContext::getTarget( const cv::Mat src,cv::Mat &dest )
{
	segmentStrategy -> segmentTarget(src,dest);
}
