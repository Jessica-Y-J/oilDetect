#pragma once
#include "segmentbase.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class ThresholdOtsu : public SegmentBase
{
public:
	ThresholdOtsu(void);
	~ThresholdOtsu(void);
	void segmentTarget(const cv::Mat inputImg,cv::Mat& resultImg) override;
};

