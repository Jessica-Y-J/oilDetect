#include "MetalImgProcessor.h"


MetalImgProcessor::MetalImgProcessor(void)
{
}


MetalImgProcessor::~MetalImgProcessor(void)
{
}

ResultOfDetect MetalImgProcessor::imgProcessor( const cv::Mat inputImg )
{
	cv::Mat binaryImg;

	//��ֵ�ָ�
	SegmentContext findTarget(otsu);
	findTarget.getTarget(inputImg,binaryImg);
	cv::namedWindow("binaryImg");
	cv::imshow("binaryImg",binaryImg);

	//Ѱ����Һ�漰��������
	cv::Mat oilPosImg;
	std::vector<cv::Rect> oilArea;
	DetectRagionsContext searchValueRegions(metalImg);
	oilArea = searchValueRegions.getValueRegions(binaryImg,oilPosImg);
	int centerX = oilArea[0].x + oilArea[0].width / 2;  //��Һ������λ��
	int centerY = oilArea[0].y + oilArea[0].height; 
	ResultOfDetect result(oilArea[0]);
	cv::namedWindow("connectArea");
	cv::imshow("connectArea",oilPosImg);

	//��Ե��⡪����������ײ�
	cv::Mat obviousEdge = cv::Mat(oilPosImg.size(),CV_8U,cv::Scalar(0));
	DetectContext cannyObvious(2,100);
	cannyObvious.getEdge(inputImg,obviousEdge);
	cv::namedWindow("obviousEdge");
	cv::imshow("obviousEdge",obviousEdge);

	for (int i = centerY + 2;i < obviousEdge.rows; ++i)
	{
		uchar *data = obviousEdge.ptr<uchar>(i);
		if (data[centerX]==255)
		{
			result.setBottom(i);  //����ײ��߶�
			break;
		}
	}

	return result;
}
