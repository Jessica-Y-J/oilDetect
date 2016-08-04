#pragma once
#include "segmentbase.h"
#include "ThresholdOtsu.h"

class SegmentContext
{
public:
	SegmentContext(void);
	SegmentContext(int type);
	~SegmentContext(void);
	void getTarget(const cv::Mat src,cv::Mat &dest);
private:
	SegmentBase * segmentStrategy;
};

