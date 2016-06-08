#include "DetectEdgeThree.h"


DetectEdgeThree::DetectEdgeThree(void)
{
}

DetectEdgeThree::DetectEdgeThree( double threshold )
	:DetectEdge(threshold)
{
	
}


DetectEdgeThree::~DetectEdgeThree(void)
{
}

void DetectEdgeThree::detectEdge(const cv::Mat& inputImg,cv::Mat& outputImg)
{
	extern int sigma;
	extern int windsize;
	double lowThreshold = highThreshold/3;
	/*double lowThreshold = 0.1 * highThreshold;*/
	//double lowThreshold = 0.4 * highThreshold;

	if (inputImg.channels() != 3)
	{
		cv::GaussianBlur(inputImg,outputImg,cv::Size(windsize,windsize),sigma,sigma);
		cv::Canny(inputImg,outputImg,lowThreshold,highThreshold);
	}
	else
	{
		cv::Mat tmpImg = cv::Mat(inputImg.size(),CV_8U);
		cv::cvtColor(inputImg,tmpImg,CV_RGB2BGRA);
		cv::GaussianBlur(tmpImg,outputImg,cv::Size(windsize,windsize),sigma,sigma);
		cv::Canny(tmpImg,outputImg,lowThreshold,highThreshold);
	}
}
