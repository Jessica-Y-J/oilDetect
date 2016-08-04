#include "Util.h"
#include <iostream>

Util::Util(void)
{
}

Util::~Util(void)
{
}

 void Util::markPosition( cv::Mat& inputImg,ResultOfDetect result)
 {
	 int thickness = 1;
	 int lineType = 8;
	 cv::Point start;
	 cv::Point end;
	 start.y = end.y = result.getBottom().second;
	 start.x = result.getBottom().first - 10;
	 end.x = result.getBottom().first + 10;
	 DrawLine(inputImg,start,end, thickness,lineType );
	 start.y = end.y = result.getOilPos().second;
	 DrawLine(inputImg,start,end, thickness,lineType );
	 start.y = end.y = result.getTopMost().second;
	 DrawLine(inputImg,start,end, thickness,lineType );

 }

 void Util::DrawLine( cv::Mat& img,cv::Point start,cv::Point end,int thickness,int lineType )
 {
	 cv::line(img,
		 start,
		 end,
		 cv::Scalar(0,0,255),
		 thickness,
		 lineType);
 }

 void Util::printResult( const cv::Mat inputImg, ResultOfDetect result)
 {
	 double percentage = (result.getBottom().second - result.getOilPos().second + 0.0)
		 /(result.getBottom().second - result.getTopMost().second + 0.0);

	 std::cout<<"容器底部高度： "<< result.getBottom().second << " ";
	 std::cout<<"油液高度： "<< result.getOilPos().second << " ";
	 std::cout<<"容器顶部高度： "<< result.getTopMost().second << std::endl;
	 std::cout<<"油液占比： "<< percentage<<endl;

	 cv::Mat finanlResult = inputImg.clone();
	 markPosition( finanlResult,result );
	 cv::namedWindow("finanlResult");
	 cv::imshow("finanlResult",finanlResult);
 }

 void Util::FillInternalContours(cv::Mat& inputImg,double dAreaThre )
 {
	 std::vector<std::vector<cv::Point> >contours;
	 cv::Mat resutlImg = cv::Mat(inputImg.size(),CV_8U,cv::Scalar(0));
	 if (inputImg.data)
	 {
		 cv::findContours(inputImg,                 // 输入图像为二值图像
			 contours,                 // 轮廓的数组
			 CV_RETR_EXTERNAL,        // 获取外轮廓
			 CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素

		 //CV_RETR_TREE  CV_RETR_EXTERNAL/  CV_RETR_CCOMP CV_RETR_LIST
		 cv::drawContours(inputImg,contours,
			 -1,
			 cv::Scalar(255),
			 CV_FILLED);

		 std::vector<std::vector<cv::Point> >:: const_iterator itc = contours.begin();
		 while (itc != contours.end())
		 {
			 double area = cv::contourArea(*itc);
			 if (area > dAreaThre)
			 {
				 itc = contours.erase(itc);
			 }
			 else
				 itc++;
		 }

		 cv::drawContours(inputImg,contours,
			 -1,
			 cv::Scalar(255),
			 CV_FILLED,
			 CV_AA);

	 }

 }

 cv::Rect Util::bigestArea( cv::Mat& inputImg )
 {
	 std::vector<std::vector<cv::Point> >contour;
	 cv::findContours(inputImg,                 // 输入图像为二值图像
		 contour,                 // 轮廓的数组
		 CV_RETR_TREE,        // 获取外轮廓
		 CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素

	 //获取外轮廓：CV_RETER_EXTERNAL 提取所有轮廓：CV_RETR_TREE
	 int pos = 0;
	 double maxArea = cv::contourArea(contour[pos]);
	 for (int i = 0; i < contour.size();++i)
	 {
		double area = cv::contourArea(contour[i]);
		if (area > maxArea)
		{
			maxArea = area;
			pos = i;
		}
	 }

	 cv::Rect resultArea = cv::boundingRect(cv::Mat(contour[pos]));
	 //cv::Mat bigImg = tmpImg(resultArea);

	 //cv::Moments mom = cv::moments(cv::Mat(contour[pos]));
	 //cv::circle(tmpImg,cv::Point(mom.m10 / mom.m00,mom.m01 / mom.m00),
		//		2,cv::Scalar(0),2);

	 //cv::drawContours(tmpImg,contour,
		// -1,
		// cv::Scalar(0),
		// 1);

	 //cv::namedWindow("bigImg");
	 //cv::imshow("bigImg",tmpImg);
	 return resultArea;




 }

 Util::connectArea Util::detectConnection( cv::Mat &inputImg )
 {
	 connectArea contour;
	 cv::findContours(inputImg,                 // 输入图像为二值图像
		 contour,                 // 轮廓的数组
		 CV_RETR_TREE,            // 获取外轮廓
		 CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素

	 return contour;
 }

 double Util::get_maxGradient( cv::Mat& inputImg )
 {
	 extern int sigma;
	 extern int windsize;
	 double maxGradient = 0;
	 cv::Mat blurImg = cv::Mat(inputImg.rows,inputImg.cols,CV_8U,cv::Scalar(0));
	 cv::Mat grad_x = cv::Mat(inputImg.rows,inputImg.cols,CV_8U,cv::Scalar(0));
	 cv::Mat grad_y = cv::Mat(inputImg.rows,inputImg.cols,CV_8U,cv::Scalar(0));
	 if (inputImg.channels() != 3)
	 {
		 cv::GaussianBlur(inputImg,blurImg,cv::Size(windsize,windsize),sigma,sigma);
		 cv::Sobel(blurImg,grad_x,inputImg.depth(),1,0,3);
		 cv::convertScaleAbs(grad_x,grad_x);
		 cv::Sobel(blurImg,grad_y,inputImg.depth(),0,1,3);
		 cv::convertScaleAbs(grad_y,grad_y);
	 }
	 else
	 {
		 cv::Mat tmpImg = cv::Mat(inputImg.rows,inputImg.cols,CV_8U,cv::Scalar(0));
		 cv::cvtColor(inputImg,tmpImg,CV_RGB2BGRA);
		 cv::GaussianBlur(tmpImg,blurImg,cv::Size(windsize,windsize),sigma,sigma);
		 cv::Sobel(blurImg,grad_x,inputImg.depth(),1,0,3);
		 cv::convertScaleAbs(grad_x,grad_x);
		 cv::Sobel(blurImg,grad_y,inputImg.depth(),0,1,3);
		 cv::convertScaleAbs(grad_x,grad_x);
	 }
	 
	 	for (int i = 0;i < grad_x.rows;++i)
	 	{
	 		uchar *data_x = grad_x.ptr<uchar>(i);
			uchar *data_y = grad_x.ptr<uchar>(i);

			for (int j = 0;j < grad_x.cols;++j)
			{
				double tmpGradient = sqrt(double(data_x[j] * data_x[j] + data_y[j] * data_y[j]));
				if (tmpGradient > maxGradient)
				{
					maxGradient = tmpGradient;
				}
			}
	 	}

		return maxGradient;
 }

 double Util::find_maxGradient( cv::Mat& inputImg )
 {
	 cv::Mat tmpImg = inputImg.clone();
	 int lastThresh = 0;
	 double highThresh = 0;
	 for (double i = 900; i >= 100;i = i - 50)
	 {
		 highThresh = 0.1 * i;
		 cv::Mat totalEdge = cv::Mat(tmpImg.size(),CV_8U,cv::Scalar(0));
		 DetectContext cannyTotal(3,i);
		 cannyTotal.getEdge(tmpImg,totalEdge);

		 cv::Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
		 cv::Mat dilateOut;
		 cv::dilate(totalEdge,dilateOut,element,cv::Point(-1,-1),1);

		 std::vector<std::vector<cv::Point> >contour;
		 cv::findContours(tmpImg,                 // 输入图像为二值图像
			 contour,                 // 轮廓的数组
			 CV_RETR_TREE,        // 获取外轮廓
			 CV_CHAIN_APPROX_NONE);    // 获取每个轮廓的每个像素

		 if(!contour.empty())
		 {
			return ((highThresh + lastThresh) / 2);
		 }
		 lastThresh = highThresh;
	 }
 }

 void Util::minBlur( cv::Mat inputImg, cv::Mat &outputImg,int maskSize )
 {
   //边界不做处理

	 //确定参考点位置
	 int referV = 0;
	 int referH = 0;
	 if (maskSize % 2 == 0)
		 referV = referH = maskSize / 2 - 1;
	 else
		 referV = referH = maskSize / 2 ;

	 int height = inputImg.rows;
	 int width = inputImg.cols;

	 for (int i = referV;i < height - (maskSize - referV); ++i)
	 {
		 for (int j = referH; j < width - (maskSize - referH);++j)
		 {
			 cv::Rect box;
			 cv::Mat tmpBox;
			 double minValue = 256;
			 box.x = j;
			 box.y = i;
			 box.height = box.width = maskSize;
			 tmpBox = inputImg(box);
			 cv::minMaxLoc(tmpBox,&minValue);
			 outputImg.at<uchar>(i,j) = minValue;
		 }
	 }
 }

 bool Util::sortConnectArea( std::vector<cv::Point> &first,std::vector<cv::Point> &second )
 {
	 if (cv::contourArea(first) > cv::contourArea(second))
		return true;

	 return false;
 }

 void Util::printDailResult( cv::Mat inputImg, ResultOfDetect result )
 {
	 cv::Point center;
	 center.x = result.getTopMost().second;
	 center.y = result.getTopMost().first;

	 cv::Point start;
	 start.x = result.getBottom().second;
	 start.y = result.getBottom().first;

	 cv::Point targetPos;
	 targetPos.x = result.getOilPos().second;
	 targetPos.y = result.getOilPos().first;

	 cv::Mat finanlResult = inputImg.clone();
	 
	 Util::DrawLine(finanlResult,center,start,2,8);
	 Util::DrawLine(finanlResult,center,targetPos,2,8);
	 cv::imshow("finanlResult",finanlResult);
	 std::cout << "result: "<< result.getPercent() << "Kpa" << std::endl;
 }
