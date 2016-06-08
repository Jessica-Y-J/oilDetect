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

float para = 1.1; //����ֵ1.1����ѡ��Χ1.1-1.3


int sigma = 2;
int windsize = 7;
int typeOfDetect = 0;//��⣬0=����1=Բ,2=����Բ�ף�3=�����ͼ 

//�����������Ա����ͼ�����ɫ�ָ���ֵ
//��԰���ҹ�������ͼ������ֵ3-8�������ĵ�ͼ20-30
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

	originalImg = imread("error\\110kVI��ĸ�ߵ�ѹ������-B����λ-2016-03-25-14-44-37.jpg"); // Read the file

	if(!originalImg.data ) // Check for invalid input
	{
		cout << "Could not open or find the originalImg" << endl ;
		return -1;
	}
	// ����ȡͼ���

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

		b2detector b2; //��λ��
		Rect rectout;
		b2(image_gray, rectout);
		image2 = originalImg(rectout);
		type = onticImg;
		//imwrite("error.jpg",image2);
	}
	else
	{
		InsDetector detector1; 

		detector1.init(); // ��ʼ��
		Rect bbox1;
		bbox1 = detector1.detect(originalImg, typeOfDetect);  // ��⣬0=����1=Բ
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
	// �������
	namedWindow("Display1", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display1", originalImg); // Show our originalImg inside it.
	namedWindow("Display2", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display2", image2); // Show our originalImg inside it.
	// �������


	//������ 
	ImgProcessorContext processImg(type);
	ResultOfDetect result = processImg.getResult(image2);
	
	//����������Լ�ͼʾ���
	if (result.getBottom().second == 0)
	{
		cout << "Fail to detect,Please try other image!" << endl;
		return 0;
	}
	Util::printResult(image2,result);
	waitKey();
	return 0;
}