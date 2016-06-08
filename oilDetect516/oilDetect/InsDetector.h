// version 0.23

#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

//#include <iostream>
//#include <string>
#include <vector>
//#include <math.h>

using namespace cv;
//using namespace std;

class InsDetector
{
public:
	InsDetector(){}
	~InsDetector(){}

	bool init(); // initial
	Rect detect(const Mat& imgin, const int IDmode = 0); // detect

private:
	CascadeClassifier cascadeB; // type B
	CascadeClassifier cascadeC; // type C
};