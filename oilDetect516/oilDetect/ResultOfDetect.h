#pragma once

#include <highgui.h>
#include <cv.h>  
#include <cxcore.h> 
#include <vector>
#include <utility>
#include "dllfunction.h"

//用于容器顶部，底部，以及油面的高度信息
class ResultOfDetect
{
public:
	ResultOfDetect(void);
	ResultOfDetect(cv::Rect oilRecArea);
	ResultOfDetect(int centerX,int in_bottom,int in_oilpos,int in_topMost);
	ResultOfDetect(TuoYuan OutValue);
	void setBottom(int height);
	void setPos(int height);
	void setTop(int height);
	void setCenterX(int centerX);
	std::pair<int,int> getBottom();
	std::pair<int,int> getOilPos();
	std::pair<int,int> getTopMost();
	~ResultOfDetect(void);
private:
	std::pair<int, int> bottom;
	std::pair<int, int> oilpos;
	std::pair<int, int> topMost;
};

