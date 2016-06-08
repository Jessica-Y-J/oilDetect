#pragma once
#include "DetectEdge.h"
#include "DetectEdgeThree.h"
#include "DetectEdgeTwo.h"
class DetectContext
{
public:
	DetectContext(void);
	DetectContext(int type, double threshold);
	void getEdge(const cv::Mat& inputImg,cv::Mat &outputImg);
	~DetectContext(void);
private:
	DetectEdge*  DC;
};

