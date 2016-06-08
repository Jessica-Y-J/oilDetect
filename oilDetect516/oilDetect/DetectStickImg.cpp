#include "DetectStickImg.h"
#include <algorithm>


DetectStickImg::DetectStickImg(void)
{
}


DetectStickImg::~DetectStickImg(void)
{
}

std::vector<cv::Rect> DetectStickImg::destArea( const cv::Mat& testImg, cv::Mat& resutlImg )
{
	cv::Mat tmpImg = testImg.clone();
	std::vector<std::vector<cv::Point> >contour;
	resutlImg = cv::Mat(tmpImg.size(),CV_8U,cv::Scalar(255));
	cv::findContours(tmpImg,                 // 输入图像为二值图像
		             contour,                 // 轮廓的数组
		             CV_RETR_TREE,        // 获取外轮廓
		             CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素

	
	std::sort(contour.begin(),contour.end(),compareSize); //按照点数从大到小排序

	std::vector<std::vector<cv::Point> >:: const_iterator itc = contour.begin()+ 2;
	while (itc != contour.end())
	{
		itc = contour.erase(itc);

	} //滤掉多余的连通分量

	std::vector<cv::Rect> oilArea; 
	cv::Rect flagArea;
	//std::cout << contour.size() << std::endl;
	for (int i = 0;i < 2;++i)
	{
		flagArea = cv::boundingRect(cv::Mat(contour[i]));
		oilArea.push_back(flagArea);
	}

	cv::drawContours(resutlImg,contour,
		-1,
		cv::Scalar(0),
		1);
	return oilArea;
}

bool DetectStickImg::compareSize( std::vector<cv::Point> &first,std::vector<cv::Point> &second )
{
	if (first.size() > second.size())
	{
		return true;
	}

	return false;
}
