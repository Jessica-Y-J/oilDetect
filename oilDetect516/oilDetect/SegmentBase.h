#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class SegmentBase
{
public:
	SegmentBase(void);
	~SegmentBase(void);

	virtual void segmentTarget(const cv::Mat inputImg,cv::Mat& resultImg) = 0;
};

