#pragma once
#include "Detectedge.h"
class DetectEdgeTwo :
	public DetectEdge
{
public:
	DetectEdgeTwo(void);
	DetectEdgeTwo(double threshold);
	~DetectEdgeTwo(void);
	void detectEdge(const cv::Mat& inputImg,cv::Mat& outputImg) override;
};

