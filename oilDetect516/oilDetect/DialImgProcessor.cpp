#include "DialImgProcessor.h"


DialImgProcessor::DialImgProcessor(void)
{
}


DialImgProcessor::~DialImgProcessor(void)
{
}

ResultOfDetect DialImgProcessor::imgProcessor(const cv::Mat inputImg )
{
	//预处理--直方图均衡以及图像高度统一化
	Mat grayImg;
	if (inputImg.channels() == 3)
	{
		
	}
}
