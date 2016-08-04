#pragma once
#include "imgprocessor.h"

#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <utility>
#include <cmath>
#include "ResultOfDetect.h"
#include "Util.h"
#include "DetectContext.h"
#include "SegmentContext.h"
#include "DetectRagionsContext.h"


enum posType{up,down,total};
enum Ostu2ImgType{initImg,DailImg,StripeImg};
class DialImgProcessor :
	public ImgProcessor
{
public:
	DialImgProcessor(void);
	~DialImgProcessor(void);
	ResultOfDetect imgProcessor(const cv::Mat inputImg);
	cv::Mat drawMaxArea(cv::Mat inputImg);
	std::vector<std::pair<int,int> > storeOutEdge(cv::Mat &inputImg);
	std::vector<vector<double> > getData2fitEllipse(std::vector<std::pair<int,int> > outEdge);
	std::vector<double> solutionLinearEqu(std::vector<std::vector<double> > matrixOfCoeff, std::vector<double> offsetVec);
	std::vector<double> getBestEllipseLS(std::vector<std::vector<double> > matrixOfCoeffA);
private:
	static bool compareSize(std::pair<int,int>  &first,std::pair<int,int>  &second);
	static void threshOfElipseOtsu(cv::Mat &inputImg,std::vector<double>,posType pos);
	static std::vector<double> eclipShrink(std::vector<double> originParameter,double times);
	static void stripeOtsu(cv::Mat inputImg,cv::Mat &outputImg);
	static std::vector<std::pair<int,int> > 
		storeStripePoints(cv::Mat inputImg,std::vector<double> outPara,std::vector<double> innerPara,posType pos);
	static void threshOfStripOtsu( cv::Mat &inputImg,std::vector<double> outPara,std::vector<double> innerPara,posType pos);
	static cv::Point calcElipseCenter(std::vector<double> parameter);
	static double calcSlope(cv::Point point0,cv::Point point1);
	static bool whetherOnLine( double slope,cv::Point routPoint,cv::Point testPoint );
	static double calcElipseRotation(std::vector<double> parameter);
	static double calcFieldAngle(double stretchCoeff,std::pair<int,int> start,std::pair<int,int> end,std::pair<int,int> center);
	static void initiImg2BlockProcess(cv::Mat &inputImg,int dividCoeff);
	static void dialImg2BlockProcess(cv::Mat &inputImg,std::vector<double> elipseCoeff,int dividCoeff);
	static void dialStripe2BlockProcess(cv::Mat &inputImg,std::vector<double> outElipseCoeff,std::vector<double> inElipseCoeff,int dividCoeff);
	static bool fussDecision( const cv::Mat &inputImg,int *degreeOfFuss);
};

