#include "ResultOfDetect.h"


ResultOfDetect::ResultOfDetect(void)
{
}

ResultOfDetect::ResultOfDetect(cv::Rect oilRecArea )
{
	int centerX = oilRecArea.x + oilRecArea.width/2;
	int centerY = oilRecArea.y + oilRecArea.height;
	
	oilpos.first = centerX;
	oilpos.second = centerY;

	topMost.first = centerX;
	topMost.second = oilRecArea.y;

	bottom.first = centerX;
}

ResultOfDetect::ResultOfDetect(TuoYuan OutValue)
{
	oilpos.first = OutValue.centerY;

	topMost.first = OutValue.centerY;
	topMost.second =  OutValue.top;

	bottom.first = OutValue.centerY;
	bottom.second =OutValue.bottom;
}

ResultOfDetect::ResultOfDetect( int centerX,int in_bottom,int in_oilpos,int in_topMost )
	:bottom(centerX,in_bottom),oilpos(centerX,in_oilpos),topMost(centerX,in_topMost)
{

}

ResultOfDetect::ResultOfDetect( std::pair<int,int> start, std::pair<int,int> targetPos, std::pair<int,int> center,double percent )
	:bottom(start),oilpos(targetPos),topMost(center),percent(percent)
{

}

void ResultOfDetect::setBottom(int height)
{
	bottom.second = height;
}

void ResultOfDetect::setPos( int height )
{
	oilpos.second = height;
}

void ResultOfDetect::setTop( int height )
{
	topMost.second = height;
}

void ResultOfDetect::setCenterX( int centerX )
{
	topMost.first = oilpos.first = bottom.first = centerX;
}

ResultOfDetect::~ResultOfDetect(void)
{
}

std::pair<int,int> ResultOfDetect::getBottom()
{
	return bottom;
}

std::pair<int,int> ResultOfDetect::getOilPos()
{
	return oilpos;
}

std::pair<int,int> ResultOfDetect::getTopMost()
{
	return topMost;
}

double ResultOfDetect::getPercent()
{
	return percent;
}








