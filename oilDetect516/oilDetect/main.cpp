#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "InsDetector.h" 
#include "ResultOfDetect.h"
#include "Util.h"
#include "DetectContext.h"
#include "SegmentContext.h"
#include "dllfunction.h"
#include "ImgProcessorContext.h"
#include "classifier.h"
#include "Cdetector_ver3.h"
#include "b2detector.h"

float para = 1.1; //经验值1.1，可选范围1.1-1.3


int sigma = 2;
int windsize = 7;
int typeOfDetect = 0;//检测，0=长，1=圆,2=金属圆孔，3=本体机图 

//这个参数是针对本体机图像的颜色分割阈值
//针对傍晚、夜晚拍摄的图，建议值3-8，白天拍的图20-30
int colorThreash = 5; 

//enum  typeofImg{circleImg,metalImg,stickImg,onticImg,errorImg};

using namespace cv;
using namespace std;
int main()
{
	Mat originalImg;
	Mat image2 ;
	Mat image_gray;
	typeofImg type;

	originalImg = imread("error\\110kVI段母线电压互感器-B相油位-2016-03-25-14-44-37.jpg"); // Read the file

	if(!originalImg.data ) // Check for invalid input
	{
		cout << "Could not open or find the originalImg" << endl ;
		return -1;
	}
	// ↑读取图像↑

	cvtColor(originalImg, image_gray, CV_BGR2GRAY);

	// ==============================
	if (typeOfDetect == 2)
	{
		type = metalImg;
		para = 0.9;
		Cdetector3 detectorV3;
		vector<Rect> rect_vector;
		detectorV3(image_gray, rect_vector);
	// ==============================

		if (rect_vector.size() > 1)
		{
			cout << "not method to desolve this kind of image at present" << endl;
			return 0;
		}
		if (rect_vector.empty())
		{
			cout << "fail to detect container" << endl;
			return 0;
		}

		Rect bbox1 = rect_vector[0];
		image2 = originalImg(bbox1);
	}
	else if (typeOfDetect == 3)
	{

		b2detector b2; //定位器
		Rect rectout;
		b2(image_gray, rectout);
		image2 = originalImg(rectout);
		type = onticImg;
		//imwrite("error.jpg",image2);
	}
	else
	{
		InsDetector detector1; 

		detector1.init(); // 初始化
		Rect bbox1;
		bbox1 = detector1.detect(originalImg, typeOfDetect);  // 检测，0=长，1=圆
		if (typeOfDetect == 1)
		{	

			bbox1.x = bbox1.x - bbox1.height / 4;
			bbox1.y = bbox1.y - bbox1.height / 4;
			bbox1.height = 1.5 * bbox1.height; 
			bbox1.width = 1.5 * bbox1.width; 
			type = circleImg;
		}
		else
			type = stickImg;
		
		image2 = originalImg(bbox1);
	}
	
	if (image2.rows < 120)
	{
		cout<<"Fail to find the target area! Please take a new photo." << endl;
		return 0;
	}
	// ↓输出↓
	namedWindow("Display1", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display1", originalImg); // Show our originalImg inside it.
	namedWindow("Display2", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display2", image2); // Show our originalImg inside it.
	// ↑输出↑


	//处理器 
	ImgProcessorContext processImg(type);
	ResultOfDetect result = processImg.getResult(image2);
	
	//最后结果计算以及图示结果
	if (result.getBottom().second == 0)
	{
		cout << "Fail to detect,Please try other image!" << endl;
		return 0;
	}
	Util::printResult(image2,result);
	waitKey();
	return 0;
}