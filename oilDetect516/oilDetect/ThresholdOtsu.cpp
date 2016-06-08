#include "ThresholdOtsu.h"


ThresholdOtsu::ThresholdOtsu(void)
{
}


ThresholdOtsu::~ThresholdOtsu(void)
{
}

void ThresholdOtsu::segmentTarget( const cv::Mat src,cv::Mat &resultImg)
{
	extern float para;
	cv::Mat tmpImg;
	if (src.channels() == 3)
	{
		cv::cvtColor(src,tmpImg,CV_RGB2GRAY);
	}
	else
		tmpImg = src.clone();
	resultImg = cv::Mat(tmpImg.rows,tmpImg.cols,CV_8U,cv::Scalar(0));
	int width = tmpImg.cols;
	int height = tmpImg.rows;	

	//histogram
	float histogram[256] = {0};
	for(int i = 0; i < height; i++) {
		unsigned char *p = tmpImg.ptr<unsigned char>(i);
		for(int j = 0; j < width; j++) 
		{
			histogram[*p++]++;
		}
	}

	//normalize histogram
	int size = height * width;
	for(int i = 0;i < 256;i++) {
		histogram[i] = histogram[i] / size;
	}

	//average pixel value
	float avgValue = 0;
	for(int i = 0;i < 256;i++) {
		avgValue += i * histogram[i];
	}

	int threshold;	
	float maxVariance = 0;
	float w = 0,u = 0;
	for(int i = 0;i < 256;i++) {
		w += histogram[i];
		u += i*histogram[i];

		float t = avgValue * w - u;
		float variance=t * t / (w * (1 - w));
		if(variance>maxVariance) {
			maxVariance=variance;
			threshold = i;
		}
	}
	
	cv::threshold(tmpImg,resultImg,para*threshold,255,cv::THRESH_BINARY);
}
