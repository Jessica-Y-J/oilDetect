#include "DialImgProcessor.h"


DialImgProcessor::DialImgProcessor(void)
{
}


DialImgProcessor::~DialImgProcessor(void)
{
}

ResultOfDetect DialImgProcessor::imgProcessor(const cv::Mat inputImg )
{
	//Ԥ����--ֱ��ͼ�����Լ�ͼ��߶�ͳһ��
	Mat grayImg;
	if (inputImg.channels() == 3)
	{
		
	}
}
