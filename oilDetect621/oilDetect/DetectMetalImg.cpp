#include "DetectMetalImg.h"


DetectMetalImg::DetectMetalImg(void)
{
}


DetectMetalImg::~DetectMetalImg(void)
{
}

std::vector<cv::Rect> DetectMetalImg::destArea( const cv::Mat& testImg, cv::Mat& resutlImg )
{
	std::vector<std::vector<cv::Point> >contour;
	resutlImg = cv::Mat(testImg.size(),CV_8U,cv::Scalar(255));
	cv::findContours(testImg,                  // 输入图像为二值图像
					 contour,                  // 轮廓的数组
					 CV_RETR_CCOMP,             // 获取外轮廓
					 CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素


	size_t maxPos = 0;
	for (size_t i = 0;i < contour.size();++i)
	{
		if (contour[i].size() > contour[maxPos].size())
		{
			maxPos = i;
		}
	}
	int maxLength = contour[maxPos].size();

	std::vector<std::vector<cv::Point> >:: const_iterator itc = contour.begin();
	while (itc != contour.end())
	{
		if (itc -> size() == maxLength)
		{
			itc = contour.erase(itc);
		}
		else if (itc -> size() < testImg.rows / 10)
		{
			itc = contour.erase(itc);
		}
		else
			itc++;
	}

	std::vector<cv::Rect> oilArea;
	cv::Rect flagArea ;
	std::vector<cv::Rect> metalBars;
	std::vector<int> collecHeight;
	for (int i = 0;i < contour.size();++i)
	{
		flagArea = cv::boundingRect(cv::Mat(contour[i]));
		metalBars.push_back(flagArea);
	}

	detectBars(metalBars);

	int centralBar_pos = 0;
	int tmpHeight = 0;
	for (int i = 0;i < metalBars.size();i++)
	{
		if (metalBars[i].height > tmpHeight)
		{
			centralBar_pos = i;
			tmpHeight = metalBars[i].height;
		}
	}

	oilArea.push_back(metalBars[centralBar_pos]);

	cv::drawContours(resutlImg,contour,
		-1,
		cv::Scalar(0),
		1);
	rectangle(resutlImg, oilArea[0], cv::Scalar( 0, 255, 0 ), 2, 8, 0 );
	return oilArea;
}

void DetectMetalImg::detectBars( std::vector<cv::Rect> &metalBars )
{
	std::vector<int> collecHeight;
	for (int i = 0;i < metalBars.size();++i)
	{
		collecHeight.push_back(metalBars[i].y + metalBars[i].height);
	}

	int average = averPos(collecHeight);
	int meanOffset = averOffset(collecHeight);

	if (meanOffset < 10)
	{
		return;
	}

	std::vector<cv::Rect>::iterator itm = metalBars.begin();

	for (int i = 0;i < collecHeight.size();++i)
	{
		if (abs(collecHeight[i] - average) >= meanOffset)
		{
			itm = metalBars.erase(itm);
		}
		else
			itm++;
	}

	detectBars(metalBars);
}

int DetectMetalImg::averPos( std::vector<int> collecHeight )
{
	int length = collecHeight.size();
	int sum = 0;
	if (length > 1)
	{
		for (int i = 0;i < length;++i)
		{
			sum += collecHeight[i];
		}
	}
	return (sum / length);
}

int DetectMetalImg::averOffset( std::vector<int> collecHeight )
{
	int average = averPos(collecHeight);
	int sumOffset = 0;
	int length = collecHeight.size();
	if (average)
	{
		for (int i = 0;i < length;++i)
		{
			sumOffset += abs(collecHeight[i] - average);
		}
	}
	return (sumOffset / length);
}
