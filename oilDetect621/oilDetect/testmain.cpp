#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "b2detector.h"

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat img;
	Mat img_gray;

	img = imread("testImg\\b2 (2).jpg");
	if (!img.data)
	{
		cout<<"sorry no img" << endl;
		return 0;
	}
	cvtColor(img, img_gray, CV_BGR2GRAY);

	// ==============================
	b2detector b2;
	Rect rectout;
	b2(img_gray, rectout);
	// ==============================

	rectangle(img, rectout, Scalar(0, 255, 255), 2, 8, 0);
	namedWindow("result", 1);
	imshow("result", img);

	waitKey();
	return 0;
}