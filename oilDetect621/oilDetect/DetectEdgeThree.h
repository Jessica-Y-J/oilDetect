#pragma once
#include "Detectedge.h"
class DetectEdgeThree :
	public DetectEdge
{
public:
	DetectEdgeThree(void);
	DetectEdgeThree(double threshold);
	~DetectEdgeThree(void);
	void detectEdge(const cv::Mat& inputImg,cv::Mat& outputImg) override;
};

