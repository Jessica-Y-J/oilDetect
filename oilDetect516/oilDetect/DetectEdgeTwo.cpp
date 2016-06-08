#include "DetectEdgeTwo.h"


DetectEdgeTwo::DetectEdgeTwo(void)
{
}

DetectEdgeTwo::DetectEdgeTwo( double threshold )
	:DetectEdge(threshold)
{
	
}


DetectEdgeTwo::~DetectEdgeTwo(void)
{
}

void DetectEdgeTwo::detectEdge( const cv::Mat& inputImg,cv::Mat& outputImg)
{
	extern int sigma;
	extern int windsize;
	double lowThreshold = highThreshold / 2;
	if (inputImg.channels() != 3)
	{
		cv::GaussianBlur(inputImg,outputImg,cv::Size(windsize,windsize),sigma,sigma);
		cv::Canny(outputImg,outputImg,lowThreshold,highThreshold);
	}
	else
	{
		cv::Mat tmpImg = cv::Mat(inputImg.size(),CV_8U);
		cv::cvtColor(inputImg,tmpImg,CV_RGB2BGRA);
		cv::GaussianBlur(tmpImg,outputImg,cv::Size(windsize,windsize),sigma,sigma);
		cv::Canny(outputImg,outputImg,lowThreshold,highThreshold);
	}
}
