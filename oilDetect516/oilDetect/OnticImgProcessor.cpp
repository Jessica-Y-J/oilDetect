#include "OnticImgProcessor.h"


OnticImgProcessor::OnticImgProcessor(void)
{
}


OnticImgProcessor::~OnticImgProcessor(void)
{
}

ResultOfDetect OnticImgProcessor::imgProcessor( const cv::Mat inputImg )
{
	cv::Mat colorSegImg = cv::Mat(inputImg.rows,inputImg.cols,CV_8U,cv::Scalar(0));
	//颜色分割
	extern int colorThreash;
	if (inputImg.channels() == 3)
	{
		cv::Mat tmpImg = inputImg.clone();


		for (int i = 0;i < tmpImg.rows; ++i)
		{
			for (int j = 0; j < tmpImg.cols; ++j)
			{
				int r = tmpImg.at<cv::Vec3b>(i,j)[2];
				int g = tmpImg.at<cv::Vec3b>(i,j)[1];
				if (r - g > colorThreash)
				{
					colorSegImg.at<uchar>(i,j) = 255;
				}
			}
		}
	}
	cv::namedWindow("colorSegImg");
	cv::imshow("colorSegImg",colorSegImg);


	std::vector<std::vector<cv::Point> >contour;
	cv::findContours(colorSegImg,                 // 输入图像为二值图像
		contour,                 // 轮廓的数组
		CV_RETR_TREE,        // 获取外轮廓
		CV_CHAIN_APPROX_NONE); 

	std::vector<cv::Rect> connectArea; 
	cv::Rect flagArea;
	for (int i = 0;i < contour.size();++i)
	{
		flagArea = cv::boundingRect(cv::Mat(contour[i]));
		connectArea.push_back(flagArea);
	}

	int pos = 0;
	for (int i = 0;i < connectArea.size() - 1;++i)
	{
		for (int j = connectArea.size() - 1;j > i;--j)
		{
			int presentArea = connectArea[j].height * connectArea[j].width;
			int previousArea = connectArea[j - 1].height * connectArea[j - 1].width;
			if (presentArea > previousArea)
			{
				swap(&connectArea[j],&connectArea[j - 1]);
			}
		}

		if (connectArea[i].width > connectArea[i].height)
		{
			pos = i;
			break;
		}
	}

	ResultOfDetect result;
	result.setCenterX(inputImg.cols/2);
	result.setTop(0);
	result.setBottom(inputImg.rows - 1);
	result.setPos(connectArea[pos].y + connectArea[pos].height / 2);

	return result;

}

void OnticImgProcessor::swap( cv::Rect *first,cv::Rect *second )
{
	cv::Rect tmp = *first;
	*first = *second;
	*second = tmp;
}
