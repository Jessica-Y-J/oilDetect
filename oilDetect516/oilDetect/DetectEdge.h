#pragma once
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <cv.h>  
#include <cxcore.h> 
class DetectEdge
{
public:
	DetectEdge(void);
	DetectEdge(double threshold);
	~DetectEdge(void);
	 virtual void detectEdge(const cv::Mat& inputImg,cv::Mat& outputImg) = 0;
protected:
	double highThreshold;
};

