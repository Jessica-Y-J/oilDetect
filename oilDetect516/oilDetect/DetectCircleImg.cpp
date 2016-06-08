#include "DetectCircleImg.h"


DetectCircleImg::DetectCircleImg(void)
{
}


DetectCircleImg::~DetectCircleImg(void)
{
}

std::vector<cv::Rect> DetectCircleImg::destArea(const cv::Mat& testImg, cv::Mat& resutlImg )
{
	cv::Mat tmpImg = testImg.clone();
	std::vector<std::vector<cv::Point> >contour;
	resutlImg = cv::Mat(tmpImg.size(),CV_8U,cv::Scalar(255));
	cv::findContours(tmpImg,                 // 输入图像为二值图像
					 contour,                 // 轮廓的数组
					 CV_RETR_TREE,        // 获取外轮廓
					 CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素

	////std::vector<int> offset_y;
	////for (int i = 0;i < contour.size();++i)
	////{
	////	int sum = 0;
	////	int aver = 0;
	////	for (int j = 0;j < contour[i].size();++j)
	////	{
	////		sum += contour[i][j].y;
	////	}
	////	aver = sum / contour[i].size();
	////	sum = 0;
	////	for (int j = 0;j < contour[i].size();++j)
	////	{
	////		sum += abs(contour[i][j].y - aver);
	////	}
	////	offset_y.push_back(sum / contour[i].size());
	////}

	////std::vector<std::vector<cv::Point> >:: const_iterator itc = contour.begin();
	////int pos = 0;
	////while (itc != contour.end())
	////{
	////	if (itc -> size() < testImg.rows / 2  || offset_y[pos] < 4)
	////	{
	////		itc = contour.erase(itc);
	////	}
	////	else
	////		itc++;
	////	pos++;
	////}

	std::vector<cv::Rect> oilArea;


	cv::drawContours(resutlImg,contour,
		-1,
		cv::Scalar(0),
		1);

	if (oilArea.size() != 0)
	{
		oilArea.clear();
	}

	return oilArea;
}
