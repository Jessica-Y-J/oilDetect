// ver 1.0.0

#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class b2detector
{
public:
	void operator()(Mat& imgin, Rect& rectout);
private:
	double autothresh(Mat& imgin);
	Mat cumsum(Mat& arrin);
	Rect BWanalyze(Mat& BWin, Point offsets);
};