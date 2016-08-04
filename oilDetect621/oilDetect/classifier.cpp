#include "classifier.h"


classifier::classifier(void)
{
}


classifier::~classifier(void)
{
}

typeofImg classifier::classify( const cv::Mat testImg )
{
	cv::Mat tmpImg;
	if (testImg.channels() == 3)
	{
		cv::cvtColor(testImg,tmpImg,CV_RGB2GRAY);
	}
	else
		tmpImg = testImg.clone();
	int height = tmpImg.rows;
	int width = tmpImg.cols;

	if ((height + 0.0 )/ width > 2)
	{
		return stickImg;
	}

	bool flag = circleOrMetal(tmpImg);
	if (flag)
	{
		return circleImg;
	}


	return metalImg;


}

bool classifier::circleOrMetal( const cv::Mat testImg )
{
	enum segType{otsu};

	//阈值分割
	cv::Mat binaryImg;
	SegmentContext findTarget(otsu);
	findTarget.getTarget(testImg,binaryImg);
	cv::namedWindow("binaryImg");
	cv::imshow("binaryImg",binaryImg);


	std::vector<std::vector<cv::Point> >contour;
	cv::Mat resutlImg = cv::Mat(testImg.size(),CV_8U,cv::Scalar(255));
	cv::findContours(binaryImg,                  // 输入图像为二值图像
		contour,                  // 轮廓的数组
		CV_RETR_LIST,             // 获取外轮廓
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
		else if (itc -> size() < testImg.rows / 4)
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

	if (metalBars.size() < 5)
	{
		return true;
	}
	
	return false;

}

void classifier::detectBars( std::vector<cv::Rect> &metalBars )
{
	std::vector<int> collecHeight;
	for (int i = 0;i < metalBars.size();++i)
	{
		collecHeight.push_back(metalBars[i].y + metalBars[i].height);
	}

	int average = averPos(collecHeight);
	int meanOffset = averOffset(collecHeight);

	if (meanOffset < 5)
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

int classifier::averPos( std::vector<int> collecHeight )
{
	int length = collecHeight.size();
	//std::cout << "length" << length << std::endl;
	int sum = 0;
	if (length > 4)
	{
		for (int i = 0;i < length;++i)
		{
			sum += collecHeight[i];
		}
	}
	return (sum / length);
}

int classifier::averOffset( std::vector<int> collecHeight )
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
