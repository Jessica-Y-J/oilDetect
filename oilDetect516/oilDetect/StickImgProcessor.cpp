#include "StickImgProcessor.h"


StickImgProcessor::StickImgProcessor(void)
{
}


StickImgProcessor::~StickImgProcessor(void)
{
}

ResultOfDetect StickImgProcessor::imgProcessor( const cv::Mat inputImg)
{
	enum segType{otsu};
	cv::Mat grayImg;
	cv::Mat colorSegImg = cv::Mat(inputImg.rows,inputImg.cols,CV_8U,cv::Scalar(0));

	//颜色分割
	if (inputImg.channels() == 3)
	{
		cv::Mat tmpImg = inputImg.clone();


		for (int i = 0;i < tmpImg.rows; ++i)
		{
			for (int j = 0; j < tmpImg.cols; ++j)
			{
				int r = tmpImg.at<cv::Vec3b>(i,j)[2];
				int g = tmpImg.at<cv::Vec3b>(i,j)[1];
				int b = tmpImg.at<cv::Vec3b>(i,j)[0];
				if (abs(r - g) < 7 && abs(g - b) < 20)
				{
					colorSegImg.at<uchar>(i,j) = 255;
				}
			}
		}
	}

	cv::cvtColor(inputImg,grayImg,CV_RGB2GRAY);

	cv::Mat obviousEdge = cv::Mat(grayImg.size(),CV_8U,cv::Scalar(0));
	DetectContext cannyObvious(3,100);
	cannyObvious.getEdge(inputImg,obviousEdge);
	colorSegImg = colorSegImg + obviousEdge;
	cv::namedWindow("colorSegImg");
	cv::imshow("colorSegImg",colorSegImg);

	//膨胀/孔洞填充/腐蚀
	cv::Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
	cv::Mat dilateOut;
	cv::dilate(colorSegImg,dilateOut,element,cv::Point(-1,-1),3);
	cv::namedWindow("result of dilateOut");
	cv::imshow("result of dilateOut",dilateOut);

	cv::Mat fillImg = dilateOut.clone();
	Util::FillInternalContours(fillImg,80);
	cv::namedWindow("result of fillImg");
	cv::imshow("result of fillImg",fillImg);

	cv::Mat erodOut;
	cv::erode(fillImg,erodOut,element,cv::Point(-1,-1),11);
	cv::namedWindow("result of erodOut");
	cv::imshow("result of erodOut",erodOut);


	ResultOfDetect result;
	//提取最大分量 并获取上底下底高度
	cv::Rect containerArea =  Util::bigestArea(erodOut);
	result.setCenterX(containerArea.x + containerArea.width / 2);
	result.setTop(containerArea.y);
	result.setBottom(containerArea.y + containerArea.height);
	int top = containerArea.y;

	cv::Mat oilImg = grayImg(containerArea);
	cv::namedWindow("oilImg");
	cv::imshow("oilImg",oilImg);


	int bottom = 0;
	int oilPos = 0;
	int width = grayImg.cols;
	int height = grayImg.rows;	

	cv::Mat binaryImg;
	SegmentContext findTarget(otsu);
	findTarget.getTarget(oilImg,binaryImg);
	cv::namedWindow("binaryImg");
	cv::imshow("binaryImg",binaryImg);

	int heightOfRegion = 0;
	cv::Mat connectImg;
	std::vector<cv::Rect> oilArea;
	DetectRagionsContext searchValueRegions(stickImg);
	oilArea = searchValueRegions.getValueRegions(binaryImg,connectImg);
	cv::namedWindow("connectArea");
	cv::imshow("connectArea",connectImg);

	if (oilArea[0].y < oilArea[1].y)
	{
		oilPos = (oilArea[0].y + oilArea[0].height + oilArea[1].y) / 2;
		oilPos = oilPos + top;
	}
	else
	{
		oilPos = (oilArea[1].y + oilArea[1].height + oilArea[0].y) / 2;
		oilPos = oilPos + top;
	}

	result.setPos(oilPos);

	return result;


}
