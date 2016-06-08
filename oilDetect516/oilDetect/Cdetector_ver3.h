// version 3.1.3c
// 20160504

#pragma once

#include "opencv2/imgproc/imgproc.hpp"

#include <vector>

using namespace cv;

class Cdetector3
{
public:
	Cdetector3(){};
	~Cdetector3(){};

	void operator()(Mat& image, vector<Rect>& rects);

private:
	void rating(vector<Vec3f>& rst, int& rt, double& ns);
	void rstoptm(vector<Vec3f> rst, vector<Rect>& rects, Size sz);
	bool chkal(vector<Vec3f>& rst);
	int rmax(vector<Vec3f> rst);
};