#include "CircleImgProcessor.h"


CircleImgProcessor::CircleImgProcessor(void)
{
}


CircleImgProcessor::~CircleImgProcessor(void)
{
}

ResultOfDetect CircleImgProcessor::imgProcessor( const cv::Mat inputImg )
{
	//extern int weakthresh;
	//extern int obviousthresh;
	//extern int oilThresh;
	
	cv::Mat tmpImg; //灰度图
	cv::cvtColor(inputImg,tmpImg,CV_RGB2GRAY);

	double weakthresh = Util::find_maxGradient( tmpImg );
	double oilThresh = weakthresh / 2;

	cv::Mat totalEdge = cv::Mat(tmpImg.size(),CV_8U,cv::Scalar(0));
	DetectContext cannyTotal(3,weakthresh);
	cannyTotal.getEdge(inputImg,totalEdge);
	cv::namedWindow("totalEdge");
	cv::imshow("totalEdge",totalEdge);

	//膨胀/孔洞填充/腐蚀
	cv::Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
	cv::Mat dilateOut;
	cv::dilate(totalEdge,dilateOut,element,cv::Point(-1,-1),1);
	cv::namedWindow("result of dilateOut");
	cv::imshow("result of dilateOut",dilateOut);

	cv::Mat fillImg = dilateOut.clone();
	Util::FillInternalContours(fillImg,80);
	cv::namedWindow("result of fillImg");
	cv::imshow("result of fillImg",fillImg);

	cv::Mat erodOut;
	cv::erode(fillImg,erodOut,element,cv::Point(-1,-1),3);
	cv::namedWindow("result of erodOut");
	cv::imshow("result of erodOut",erodOut);

	ResultOfDetect result;
	//提取最大分量
	cv::Rect containerArea =  Util::bigestArea(erodOut);
	if (containerArea.height / (containerArea.width+0.0) < 1 ||containerArea.height / (containerArea.width+0.0) > 1.3 )
	{
		return result;
	}

	result.setCenterX(containerArea.x + containerArea.width / 2);
	result.setTop(containerArea.y + 8 * containerArea.height / 33);
	result.setBottom(containerArea.y + containerArea.height - 8 * containerArea.height / 33);

	cv::Rect newArea;
	newArea.x = containerArea.x + 12 * containerArea.width /33;
	newArea.y = containerArea.y + 8 * containerArea.height / 33;
	newArea.height= 17 * containerArea.height /33;
	newArea.width = 9 * containerArea.width / 33;

	cv::Mat oilImg = tmpImg(newArea);
	cv::namedWindow("oilImg");
	cv::imshow("oilImg",oilImg);

	cv::Mat tmpResult;
	DetectContext cannyOil(3,oilThresh);
	cannyOil.getEdge(oilImg,tmpResult);

	cv::Mat newElement = getStructuringElement(cv::MORPH_RECT,cv::Size(2,1));
	cv::erode(tmpResult,tmpResult,newElement,cv::Point(-1,-1),2);
	cv::namedWindow("tmpResult");
	cv::imshow("tmpResult",tmpResult);

	connectArea contour = Util::detectConnection(tmpResult);
	std::vector<std::pair<int,double> >info_contour = mergeConnection(contour);
	std::sort(info_contour.begin(),info_contour.end(),compConnection);
	
	std::vector<int> offset;
	
	for (int i = 0;i < 10 && i <info_contour.size(); i++)
	{
		int sumOffset = 0;
		for (int j = 0;j < 10 && i <info_contour.size();j++)
		{
			sumOffset =sumOffset + sqrt(double(abs(info_contour[i].first - info_contour[j].first)
				* abs(info_contour[i].first - info_contour[j].first)));
		}
		offset.push_back(sumOffset);
	}

	int pos = 0;
	for (int i = 0;i < 10 && i < offset.size();i++)
	{
		if (offset[i] < offset[pos])
		{
			pos = i;
		}
	}

	int tmpHeight = info_contour[0].first + result.getTopMost().second;
	result.setPos(tmpHeight);

	return result;
}

void CircleImgProcessor::storeContainerPixels(const cv::Mat containerImg,int *Xaxis,int *Yaxis,int &EqualNum)
{
	int width = containerImg.cols;
	int height = containerImg.rows;	
	std::vector<int> heightAxes;
	std::vector<int> lineAxes;
	EqualNum = 0;
	for(int i = 0; i < height; i++) {
		const unsigned char *p = containerImg.ptr<unsigned char>(i);
		for(int j = 0; j < width; j++) 
		{
			if (p[j] == 0)
			{
				heightAxes.push_back(i);
				lineAxes.push_back(j);
				EqualNum++;
			}
		}
	}

	Xaxis =new int[EqualNum];
	Yaxis =new int[EqualNum];
	for (int i = 0;i < EqualNum;i++)
	{
		Xaxis[i] = heightAxes[i];
		Yaxis[i] = lineAxes[i];
	}
}

std::vector<std::pair<int,double> >
	CircleImgProcessor::mergeConnection( connectArea &contour )
{

	std::vector<std::pair<int,double> > info_contour;

	int sumHeight = 0;
	double area = 0;
	for (int i = 0;i < contour.size();++i)
	{
		area = 0;
		for (int j = 0; j < contour[i].size(); ++j)
		{
			sumHeight += contour[i][j].y;
			area++;
		}
		info_contour.push_back( std::pair<int,double>(sumHeight / contour[i].size(), area));
		sumHeight = 0;
	}

	//int offsetHeight = 1;
	//std::vector<std::pair<int,double> >:: iterator it_start = info_contour.begin();
	//std::vector<std::pair<int,double> >:: iterator it_second = info_contour.begin();

	//while (it_start != info_contour.end())
	//{
	//	it_second = it_start + 1;
	//	while (it_second != info_contour.end())
	//	{
	//		if (abs(it_second -> first - it_start -> first) <= offsetHeight)
	//		{
	//			it_start->second = it_start->second + it_second->second;
	//			it_second = info_contour.erase(it_second);
	//		}
	//		else
	//			it_second++;
	//	}
	//	it_start++;
	//	
	//}

	return info_contour;
	
}

bool CircleImgProcessor::compConnection( std::pair<int,double>& leftArea,std::pair<int,double>& rightArea )
{
	if (leftArea.second > rightArea.second)
	{
		return true;
	}
	return false;
}
