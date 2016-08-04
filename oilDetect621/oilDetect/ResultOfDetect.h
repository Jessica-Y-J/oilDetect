#pragma once

#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <vector>
#include <utility>
#include "dllfunction.h"

//���������������ײ����Լ�����ĸ߶���Ϣ
class ResultOfDetect
{
public:
	ResultOfDetect(void);
	ResultOfDetect(cv::Rect oilRecArea);
	ResultOfDetect(int centerX,int in_bottom,int in_oilpos,int in_topMost);
	ResultOfDetect(TuoYuan OutValue);

	ResultOfDetect(std::pair<int,int> start, std::pair<int,int> targetPos, std::pair<int,int> center,double percent);

	void setBottom(int height);
	void setPos(int height);
	void setTop(int height);
	void setCenterX(int centerX);
	std::pair<int,int> getBottom();
	std::pair<int,int> getOilPos();
	std::pair<int,int> getTopMost();
	double getPercent();
	~ResultOfDetect(void);
private:
	//start��Ӧbottom targetPos��Ӧoilpos center��ӦtopMost
	std::pair<int, int> bottom;
	std::pair<int, int> oilpos;
	std::pair<int, int> topMost;
	double percent;
};

