#pragma once
#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <vector>
#include <utility>
#include "ResultOfDetect.h"
#include "DetectContext.h"
class Util
{
public:
	typedef std::vector<std::vector<cv::Point> > connectArea;
	Util(void);
	~Util(void);
	static void markPosition(cv::Mat& inputImg,ResultOfDetect result);

	static void DrawLine(cv::Mat& img,cv::Point start,cv::Point end,int thickness,int lineType);
	static void printResult(const cv::Mat inputImg, ResultOfDetect result);
	static void printDailResult(cv::Mat inputImg, ResultOfDetect result);

	static void FillInternalContours(cv::Mat& inputImg,double dAreaThre);
	static cv::Rect bigestArea(cv::Mat& inputImg);
	static connectArea detectConnection(cv::Mat &inputImg);
	static double get_maxGradient(cv::Mat& inputImg);
	static double find_maxGradient(cv::Mat& inputImg);
	static void minBlur(cv::Mat inputImg, cv::Mat &outputImg,int maskSize);
	static bool sortConnectArea(std::vector<cv::Point> &first,std::vector<cv::Point> &second);
};

