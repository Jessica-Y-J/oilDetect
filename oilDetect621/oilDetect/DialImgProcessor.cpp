#include "DialImgProcessor.h"


DialImgProcessor::DialImgProcessor(void)
{
}


DialImgProcessor::~DialImgProcessor(void)
{
}

enum segType{otsu};

ResultOfDetect DialImgProcessor::imgProcessor(const cv::Mat inputImg )
{
	bool processFlag;

	//0=����ͼ�쳣ģ�������һ�����ɿ���1=����ͼһ��ģ��������֤��� 2=����ͼ�б�֤
	int degreeOfFuss = 0; 
	char go;

	//Ԥ����--ͼ��߶�ͳһ���Լ���ֵ�ָ�
	cv::Mat grayImg;
	if (inputImg.channels() == 3)
		cv::cvtColor(inputImg,grayImg,CV_RGB2GRAY);
	else
		grayImg = inputImg.clone();

		//�߶�ͳһ
	int newWidth = grayImg.cols * 300 / grayImg.rows;
	cv::resize(grayImg,grayImg,cv::Size(newWidth,300),0,0,cv::INTER_CUBIC);

	processFlag = fussDecision(grayImg, &degreeOfFuss);
	if (!processFlag)
	{
		std::cout << "ͼ��̫ģ�����ܾ�����" << std::endl;
		ResultOfDetect result;
		return result;

	}
	else if (degreeOfFuss == 1)
	{
		std::cout << "ͼ��ģ������ѡ���Ƿ��������y/n" << std::endl;
		std::cin >> go;
		if (go == 'y')
			std::cout << "���ܱ�֤�����ȷ�ԣ������ʹ�ý����" <<std::endl;
	}
	else
		go = 'y';

	if (go != 'y')
	{
		ResultOfDetect result;
		return result;
	}

	cv::imshow("grayImg",grayImg);

	cv::Mat equalizeImg;
	cv::equalizeHist(grayImg, equalizeImg); //ֱ��ͼ����
	grayImg = equalizeImg;

		//���з�Ƭ��Ȼ���ÿƬ����Otsu,�ٺϲ�����������Χ�ĸ���
	cv::Mat segResult = grayImg.clone(); 
	initiImg2BlockProcess(segResult,3);
	imshow("segResult",segResult);
	
	//��ȡ�����ͨ����������ȡ������Ե������Բ���
	cv::Mat maxConnectArea = drawMaxArea(segResult);
		//�ռ�������Ե��
	std::vector<std::pair<int,int> > outEdge = storeOutEdge(maxConnectArea ); 
		//��ʾ������
	cv::Mat tempImg = cv::Mat(grayImg.size(),CV_8U,cv::Scalar(0));
	uchar* data = tempImg.data;
	for (int i = 0;i < outEdge.size(); ++i)
	{
		int x = outEdge[i].first;
		int y = outEdge[i].second;

		tempImg.at<uchar>(x,y) = 255;
	}
	imshow("outEdge",tempImg);
		//���������Բ��5������
	std::vector<std::vector<double> > matrixOfCoeffA = getData2fitEllipse( outEdge ); //���췽��ϵ��
	std::vector<double> answerVec = getBestEllipseLS( matrixOfCoeffA); //�����Բ������ز���


	cv::Mat captureDail = grayImg.clone();
	//��ʾ�ػ����Բ���򣬼������λ��
	for (int i = 0; i < captureDail.rows; ++i)
	{
		for ( int j = 0; j < captureDail.cols; ++j)
		{
			double judeValue = answerVec[0] * i * i + answerVec[1] * i * j + answerVec[2] * j * j 
								+ answerVec[3] * i + answerVec[4] * j + 1;
			if (judeValue > 0)
			{
				captureDail.at<uchar>(i,j) = 255;
			}
		}
	}
	imshow("captureDail",captureDail);

	 //�Դ�����ڲ����з�Ƭ��ֵ�ָ��
	cv::Mat innerSeg = captureDail.clone();
	dialImg2BlockProcess(innerSeg,answerVec,2);
	innerSeg = 255 - innerSeg; //ȡ��
	imshow("innerSeg",innerSeg);

		//��̬ѧ��������ʴ���׶���䡢����
	cv::Mat inner2morph = innerSeg.clone();
	cv::Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
	cv::erode(inner2morph,inner2morph,element,cv::Point(-1,-1),2);
	Util::FillInternalContours(inner2morph,80);
	cv::dilate(inner2morph,inner2morph,element,cv::Point(-1,-1),2);
	imshow("inner2morph",inner2morph);

	//��ȡ�����ͨ������������ɫ��ͨ��������,������߽�������ڲ���Բ���
	cv::Mat maxInnerConnection = drawMaxArea(inner2morph);
	imshow("maxInnerConnection",maxInnerConnection);

	//��Բ���
	std::vector<std::pair<int,int> > outEdgeOfInner = storeOutEdge(maxInnerConnection); //�ռ����Ե��

	//��ʾ������
	cv::Mat tempInnerImg = cv::Mat(grayImg.size(),CV_8U,cv::Scalar(0));
	for (int i = 0;i < outEdgeOfInner.size(); ++i)
	{
		int x = outEdgeOfInner[i].first;
		int y = outEdgeOfInner[i].second;

		tempInnerImg.at<uchar>(x,y) = 255;
	}
	imshow("tempInnerImg",tempInnerImg);

	//���������Բ��5������
	std::vector<std::vector<double> > matrixOfCoeffA2Inner = getData2fitEllipse( outEdgeOfInner ); //���췽��ϵ��
	std::vector<double> answerVec2Inner = getBestEllipseLS( matrixOfCoeffA2Inner); //�����Բ������ز���

	cv::Mat toShowElipse = grayImg.clone();
	//��ʾ�ػ����Բ���򣬼�С����λ��
	for (int i = 0; i < grayImg.rows; ++i)
	{
		for ( int j = 0; j < grayImg.cols; ++j)
		{
			double judeValue = answerVec2Inner[0] * i * i + answerVec2Inner[1] * i * j +answerVec2Inner[2] * j * j 
				+ answerVec2Inner[3] * i + answerVec2Inner[4] * j + 1;
			if (judeValue < 0)
			{
				toShowElipse.at<uchar>(i,j) = 255;
			}
		}
	}
	imshow("toShowElipse",toShowElipse);
	std::vector<double> middleVec = eclipShrink(answerVec2Inner,1.2);
	//��ʾ�ػ����Բ���򣬼��̶��ڱ��̱���λ��
	for (int i = 0; i < grayImg.rows; ++i)
	{
		for ( int j = 0; j < grayImg.cols; ++j)
		{
			double judeValue = middleVec[0] * i * i + middleVec[1] * i * j + middleVec[2] * j * j 
				+ middleVec[3] * i + middleVec[4] * j + middleVec[5];
			if (judeValue < 0)
			{
				toShowElipse.at<uchar>(i,j) = 255;
			}
		}
	}


	std::vector<double> outVec = eclipShrink(answerVec2Inner,1.35);
	//��ʾ�ػ����Բ���򣬼��̶������λ��
	for (int i = 0; i < grayImg.rows; ++i)
	{
		for ( int j = 0; j < grayImg.cols; ++j)
		{
			double judeValue = outVec[0] * i * i + outVec[1] * i * j + outVec[2] * j * j 
				+ outVec[3] * i + outVec[4] * j + outVec[5];
			if (judeValue > 0)
			{
				toShowElipse.at<uchar>(i,j) = 255;
			}
		}
	}

	//������ֵ�ָ�
	cv::Mat stripeSeg;
	stripeOtsu(toShowElipse,stripeSeg);
	imshow("stripeSeg",stripeSeg);
	stripeSeg = 255 - stripeSeg;

	//���������ͨ����
	cv::Mat maxStripeConnection = drawMaxArea(stripeSeg);
	imshow("maxStripeConnection",maxStripeConnection);

	int height = maxStripeConnection.rows;
	int width = maxStripeConnection.cols;

	//���ҿ̶ȱ�����ʼλ��
	std::pair<int,int> start; //�̶���ʼλ��
	//std::pair<int,int> center;
	std::pair<int,int> targetPos; //ָ��Ŀ��λ��
	for (int i = height - 1; i >= height / 2; --i)
	{
		for (int j = 0; j < width /2;++j)
		{
			if (maxStripeConnection.at<uchar>(i,j) == 255 )
			{
				start.first = i;
				start.second = j;
				i = width / 2 - 1;
				break;
			}
		}
	}

	//�ӱ��̶̿��������ҵ�ָ��λ��
		//�Ƚ���ͼ����ǿ
	//cv::Mat equalizeImg;
	//cv::equalizeHist(grayImg, equalizeImg); //ֱ��ͼ����
	cv::Mat enhanceImg = equalizeImg.clone();
	Util::minBlur(equalizeImg, enhanceImg,2); //��Сֵ�˲�
	imshow("enhanceImg",enhanceImg);

		//Ѱ�ҿ̶���������Ȧ
	std::vector<double> dailOutVec = eclipShrink(answerVec2Inner,1.03);
		//����ǿ��ͼ����ȡ���̶�����
	cv::Mat dailBand = cv::Mat(grayImg.size(),CV_8U,cv::Scalar(0));
	cv::Mat assistBand = cv::Mat(grayImg.size(),CV_8U,cv::Scalar(0));
	for (int i = 0; i < dailBand.rows; ++i)
	{
		for (int j = 0; j < dailBand.cols; ++j)
		{
			double judeValue_in = dailOutVec[0] * i * i + dailOutVec[1] * i * j + dailOutVec[2] * j * j 
									+ dailOutVec[3] * i + dailOutVec[4] * j + dailOutVec[5];

			double judeValue_out = middleVec[0] * i * i + middleVec[1] * i * j + middleVec[2] * j * j 
									+ middleVec[3] * i + middleVec[4] * j + middleVec[5];
			if (judeValue_in > 0 && judeValue_out < -0.01)
			{
				dailBand.at<uchar>(i,j) = enhanceImg.at<uchar>(i,j);
				assistBand.at<uchar>(i,j) = 255;
			}
		}
	}
	imshow("dailBand",dailBand);
	imshow("assistBand",assistBand);

		//��ָ���������з�Ƭ�����������ղ����ȵ�Ӱ�죬���ֱ������ֵ�ָ�
	cv::Mat dialSeg = dailBand.clone();
	dialStripe2BlockProcess(dialSeg,middleVec,dailOutVec,2);
	imshow("dialSeg",dialSeg);

		//���׶���¶
	cv::Mat holeOfStripe = cv::Mat(grayImg.size(),CV_8U,cv::Scalar(0));
	for (int i = 0; i < dialSeg.rows; ++i)
	{
		for (int j = 0; j < dialSeg.cols; ++j)
		{
			if (dialSeg.at<uchar>(i,j) != assistBand.at<uchar>(i,j))
				holeOfStripe.at<uchar>(i,j) = 255;
		}
	}
	imshow("holeOfStripe",holeOfStripe);
		//ͨ����ͨ�������ҽ��п׶��Ĳ���
	Util::connectArea holeContours= Util::detectConnection(holeOfStripe);
		//�Կ׶��������򣬴Ӵ�С����
	sort(holeContours.begin(),holeContours.end(),Util::sortConnectArea);
		//��ȡÿ���׶����ڵ�λ��
	std::vector<cv::Rect> connectAreaPos;
	for (int i = 0; i < holeContours.size();++i)
	{
		cv::Rect tmpRectArea = cv::boundingRect(holeContours[i]);
		connectAreaPos.push_back(tmpRectArea);
	}
		
	//ͨ���׶��������������Ƿ���ڱ߽絽��߽�һֱΪ0���ж��Ƿ�Ϊָ������λ��
	cv::Point elipseCenter = calcElipseCenter(answerVec);
	std::pair<int,int> center = std::pair<int,int>(elipseCenter.y,elipseCenter.x);
	cv::Point routPoint;
	for (int k = 0; k < connectAreaPos.size();++k)
	{
		cv::Rect selectHoleArea = connectAreaPos[k];
		routPoint.x = selectHoleArea.x + selectHoleArea.width / 2;
		routPoint.y = selectHoleArea.y + selectHoleArea.height / 2;
		double slope = calcSlope(elipseCenter,routPoint);

		for (int i = 0; i < stripeSeg.rows; ++i)
		{
			for (int j = 0; j < stripeSeg.cols;++j)
			{
				cv::Point testPoint = cv::Point(j,i);

				double judeValue_in = dailOutVec[0] * i * i + dailOutVec[1] * i * j + dailOutVec[2] * j * j 
					+ dailOutVec[3] * i + dailOutVec[4] * j + dailOutVec[5];

				double judeValue_out = middleVec[0] * i * i + middleVec[1] * i * j + middleVec[2] * j * j 
					+ middleVec[3] * i + middleVec[4] * j + middleVec[5];

				if (judeValue_in > 0 && judeValue_out < -0.01)
				{
					if (whetherOnLine(slope,routPoint,testPoint) )
					{
						if (stripeSeg.at<uchar>(i,j) == 255)
						{
							i = stripeSeg.rows;
							break;
						}
					}
				}
				else if (j == stripeSeg.cols - 1)
				{
					k = connectAreaPos.size();
					break;
				}
			}

			if (k == connectAreaPos.size())
				break;
			if (k == connectAreaPos.size() - 1)
			{
				routPoint.x = connectAreaPos[0].x + connectAreaPos[0].width / 2;
				routPoint.y = connectAreaPos[0].y + connectAreaPos[0].height / 2;
			}
		}
	}

	targetPos.first = routPoint.y;
	targetPos.second = routPoint.x;

	double stretchCoeff = calcElipseRotation(answerVec2Inner);
	double angle = calcFieldAngle(stretchCoeff,start,targetPos,center);

	ResultOfDetect result(start,targetPos,center,angle);
	Util::printDailResult(grayImg,result);
	return result;

}

cv::Mat DialImgProcessor::drawMaxArea( cv::Mat inputImg )
{
	cv::Mat resultImg = cv::Mat(inputImg.size(),CV_8U,cv::Scalar(0));
	std::vector<std::vector<cv::Point> >contour;
	cv::findContours(inputImg,                 // ����ͼ��Ϊ��ֵͼ��
		contour,                 // ����������
		CV_RETR_TREE,        // ��ȡ������
		CV_CHAIN_APPROX_NONE);    // ��ȡÿ��������ÿ������

	std::vector<std::vector<cv::Point> >::iterator it = contour.begin();

	double maxArea = 0;
	for (int i = 0; i < contour.size();++i)
	{
		double area = cv::contourArea(contour[i]);
		if (area > maxArea)
		{
			maxArea = area;
		}
	}

	while (it != contour.end())
	{
		double area = cv::contourArea(*it);
		if (area < maxArea)
			it = contour.erase(it);
		else
			it++;

	}
	cv::drawContours(resultImg,contour,
		-1,
		cv::Scalar(255),
		1);

	//cv::namedWindow("bigImg");
	//cv::imshow("bigImg",resultImg);

	return resultImg;
}

std::vector<std::pair<int,int> > 
	DialImgProcessor::storeOutEdge( cv::Mat &inputImg )
{
	std::vector<std::pair<int,int> > edgePoints;
	std::vector<std::pair<int,int> > leftEdge;
	std::vector<std::pair<int,int> > rightEdge;
	std::vector<std::pair<int,int> > recordLeft;  //�����Ե���б��
	std::vector<std::pair<int,int> > recordRight; //���ұ�Ե���б��

	//��¼����Ե
	int width = inputImg.cols;
	int height = inputImg.rows;	

		//�������Ե
	for(int i = 0; i < height; i++) {
		const unsigned char *p = inputImg.ptr<unsigned char>(i);
		for(int j = 0; j <= width /2; j++) 
		{
			if (p[j] == 255)
			{
				leftEdge.push_back(std::pair<int,int>(i,j));
				break;
			}
		}
	}
		//�����ұ�Ե
	for(int i = 0; i < height; i++) {
		const unsigned char *p = inputImg.ptr<unsigned char>(i);
		for(int j = width - 1; j >= width /2; j--) 
		{
			if (p[j] == 255)
			{
				rightEdge.push_back(std::pair<int,int>(i,j));
				break;
			}
		}
	}
		//������Ե
	int start = 0;
	int length = 1;
	for (int i = 1;i < leftEdge.size(); i++)
	{
		if (abs(leftEdge[i].second - leftEdge[i - 1].second) > 5 || i == leftEdge.size() - 1)
		{
			recordLeft.push_back(std::pair<int,int>(start,length));
			start = i;
			length = 1;
		}
		else
			length++;
	}
		//����ұ�Ե
	start = 0;
	length = 1;
	for (int i = 1;i < rightEdge.size(); i++)
	{
		if (abs(rightEdge[i].second - rightEdge[i - 1].second) > 5 || i == rightEdge.size() - 1)
		{
			recordRight.push_back(std::pair<int,int>(start,length));
			start = i;
			length = 1;
		}
		else
			length++;
	}

	sort(recordLeft.begin(),recordLeft.end(),compareSize);
	sort(recordRight.begin(),recordRight.end(),compareSize);

	for (int i = recordLeft[0].first; i < recordLeft[0].first + recordLeft[0].second;++i)
	{
		edgePoints.push_back(leftEdge[i]);
	}

	for (int i = recordRight[0].first; i < recordRight[0].first + recordRight[0].second;++i)
	{
		edgePoints.push_back(rightEdge[i]);
	}


	return edgePoints;
}

std::vector<double> 
	DialImgProcessor::solutionLinearEqu( std::vector<std::vector<double> > matrixOfCoeff, std::vector<double> offsetVec )
{
	int numberOfEquation = matrixOfCoeff.size();
	int numberOfParameter = matrixOfCoeff[0].size();

	std::vector<double> answerVec(numberOfParameter,0);

	//��Ԫ����
	for (int i = 0; i < numberOfEquation - 1;++i)  //���� 
	{
		if (matrixOfCoeff[i][i] == 0) //�ж�����Ԫ�Ƿ�Ϊ0����Ϊ0��ͨ�������ҵ�������
		{
			for (int line = i + 1;line < numberOfEquation; ++line)
			{
				if (matrixOfCoeff[line][i] != 0)
				{
					swap(matrixOfCoeff[i],matrixOfCoeff[line]);
					swap(offsetVec[i],offsetVec[line]);
					break;
				}
			}
		}
		double num2div = matrixOfCoeff[i][i];

		//��Ԫ�����й�һ��
		offsetVec[i] = offsetVec[i] / num2div;
		for (int t = i; t < numberOfParameter;++t)
		{
			matrixOfCoeff[i][t] = matrixOfCoeff[i][t] / num2div;
		}

		for (int j = i + 1; j < numberOfEquation;++j) //��ǰ��Ԫ����
		{
			if (matrixOfCoeff[j][i] == 0)
				continue;

			for (int k = i + 1; k < numberOfParameter;++k) //������Ԫ�Ĳ���
			{
				matrixOfCoeff[j][k] = matrixOfCoeff[j][k] / matrixOfCoeff[j][i] - matrixOfCoeff[i][k];
			}

			offsetVec[j] = offsetVec[j] / matrixOfCoeff[j][i] - offsetVec[i];
			matrixOfCoeff[j][i] = matrixOfCoeff[j][i] / matrixOfCoeff[j][i] - matrixOfCoeff[i][i];

		}

	}

	//�ش�����������
	for (int i = numberOfParameter - 1; i >= 0; --i)
	{
		double offset = 0;
		for (int j = i + 1;j < numberOfParameter; ++j)
		{
			offset += answerVec[j] * matrixOfCoeff[i][j];
		}

		answerVec[i] = (offsetVec[i] - offset) / matrixOfCoeff[i][i];
	}

	return answerVec;

}

std::vector<double> DialImgProcessor::getBestEllipseLS( std::vector<std::vector<double> > matrixOfCoeffA)
{
	double maxError = 6;
	std::vector<double> answerVec(5,0);

	while (maxError > 0.001)
	{
		int length = matrixOfCoeffA.size();
		std::vector<vector<double> > matrixOfCoeffAT(5,std::vector<double>(length,0));
		std::vector<vector<double> > matrixOfCoeff(5,std::vector<double>(5,0));
		std::vector<double> offset(length,-1);
		std::vector<double> offsetVec(5,0);

		for (int i = 0;i < 5;++i) //��ȡ����A��ת��
		{
			for (int j = 0;j < length;++j)
			{
				matrixOfCoeffAT[i][j] = matrixOfCoeffA[j][i];
			}
		}

		for (int i = 0;i < 5;++i) //��ȡAת����A�ĳ˷�
		{
			for (int j = 0;j < 5; ++j)
			{
				for (int k = 0;k < length; ++k)
				{
					matrixOfCoeff[i][j] += matrixOfCoeffAT[i][k]*matrixOfCoeffA[k][j];
				}
			}
		}

		for (int i = 0;i < 5;++i)
		{
			for (int j = 0;j < length;++j)
			{
				offsetVec[i] += matrixOfCoeffAT[i][j] * offset[j]; 
			}
		}

		answerVec = solutionLinearEqu(matrixOfCoeff, offsetVec);

		if (length <= 10)
			return answerVec;

		maxError = 0;
		vector<double> error(length,0);
		for (int i = 0;i < length;++i)
		{
			double residual = 0;
			for (int j = 0;j < 5;j++)
			{
				residual += matrixOfCoeffA[i][j] * answerVec[j];
			}

			residual = abs(residual + 1);

			if (residual > maxError)
			{
				maxError = residual;
			}

			error[i] = residual;
		}

		//ɾ������ĵ�Ķ�Ӧ����
		if (maxError > 0.001)
		{
			sort(error.begin(),error.end());
			double downError = error[length - 10];
			std::vector<vector<double> >::iterator it = matrixOfCoeffA.begin();
			while (it != matrixOfCoeffA.end())
			{
				double residual = 0;
				for (int j = 0;j < 5;j++)
				{
					residual +=(*it)[j] * answerVec[j];
				}

				residual = abs(residual + 1);
				if (residual >= downError)
					it = matrixOfCoeffA.erase(it);
				else
					it++;

			}
		}
	}

	return answerVec;
}

std::vector<vector<double> > DialImgProcessor::getData2fitEllipse( std::vector<std::pair<int,int> > outEdge )
{
	int length = outEdge.size();
	std::vector<vector<double> > matrixOfCoeffA(length,std::vector<double>(5,0));

	for (int i = 0;i < length; ++i) //��ȡA����
	{
		matrixOfCoeffA[i][0] = outEdge[i].first * outEdge[i].first;
		matrixOfCoeffA[i][1] = outEdge[i].first * outEdge[i].second;
		matrixOfCoeffA[i][2] = outEdge[i].second * outEdge[i].second;
		matrixOfCoeffA[i][3] = outEdge[i].first;
		matrixOfCoeffA[i][4] = outEdge[i].second;
	}
	return matrixOfCoeffA;
}

bool DialImgProcessor::compareSize( std::pair<int,int> &first,std::pair<int,int> &second )
{
	if (first.second > second.second)
	{
		return true;
	}

	return false;
}

void DialImgProcessor::threshOfElipseOtsu( cv::Mat &inputImg,std::vector<double> parameters,posType pos)
{
	if (parameters.size() == 5)
		parameters.push_back(1);
	
	std::vector<int> valueOfPixels;
	if (pos == up)
	{
		for (int i = 0; i < inputImg.rows; ++i)
		{
			for ( int j = 0; j < inputImg.cols; ++j)
			{
				double judeValue = parameters[0] * i * i + parameters[1] * i * j + parameters[2] * j * j 
					+ parameters[3] * i + parameters[4] * j + parameters[5];
				if (judeValue < 0)
				{
					int tmpValue = inputImg.at<uchar>(i,j);
					valueOfPixels.push_back(tmpValue);
				}
			}
		}
	}
	else{
		for (int i = 0; i < inputImg.rows; ++i)
		{
			for ( int j = 0; j < inputImg.cols; ++j)
			{
				double judeValue = parameters[0] * (i + inputImg.rows) * (i + inputImg.rows) 
								   + parameters[1] * (i + inputImg.rows) * j + parameters[2] * j * j 
								   + parameters[3] * (i + inputImg.rows) + parameters[4] * j + parameters[5];
				if (judeValue < 0)
				{
					int tmpValue = inputImg.at<uchar>(i,j);
					valueOfPixels.push_back(tmpValue);
				}
			}
		}
	}



	float histogram[256] = {0};
	for (int i = 0; i < valueOfPixels.size();++i)
	{
		histogram[valueOfPixels[i]]++;
	}


	//normalize histogram
	for(int i = 0;i < 256;i++) 
	{
		histogram[i] = histogram[i] / valueOfPixels.size();
	}

	//average pixel value
	float avgValue = 0;
	for(int i = 0;i < 256;i++) 
	{
		avgValue += i * histogram[i];
	}

	int threshold;	
	float maxVariance = 0;
	float w = 0,u = 0;
	for(int i = 0;i < 256;i++) 
	{
		w += histogram[i];
		u += i*histogram[i];

		float t = avgValue * w - u;
		float variance=t * t / (w * (1 - w));
		if(variance>maxVariance) 
		{
			maxVariance=variance;
			threshold = i;
		}
	}
	//std::cout << threshold << endl;
	cv::threshold(inputImg,inputImg,threshold,255,cv::THRESH_BINARY);
	//cv::imshow("otsu",inputImg);

}

std::vector<double> DialImgProcessor::eclipShrink( std::vector<double> originParameter,double times )
{
	times = 1 / times;
	double beta = 1 - times;
	double a = originParameter[0];
	double b = originParameter[1];
	double c = originParameter[2];
	double d = originParameter[3];
	double e = originParameter[4];

	//(xc,yc)��Բ��λ��
	cv::Point elipseCenter = calcElipseCenter(originParameter);
	double xc = elipseCenter.y;
	double yc = elipseCenter.x;

	std::vector<double> resultParameter(6,0);
	double unifom = beta * beta * (a * xc * yc + b * xc * yc + c * yc * yc)
					+ beta * (e * yc + d * xc);

	resultParameter[0] = a * times * times;
	resultParameter[1] = b * times * times;
	resultParameter[2] = c * times * times;
	resultParameter[3] = times * (2 * a * beta * xc + b * beta * yc + d);
	resultParameter[4] = times * (2 * c * beta * yc + b * beta * xc + e);
	resultParameter[5] = beta * beta * (a * xc * yc + b * xc * yc + c * yc * yc)
		                 + beta * (e * yc + d * xc) + 1;

	return resultParameter;
}

void DialImgProcessor::stripeOtsu( cv::Mat inputImg,cv::Mat &outputImg )
{
	std::vector<int> valueOfPixels;
	for (int i = 0; i < inputImg.rows; ++i)
	{
		for ( int j = 0; j < inputImg.cols; ++j)
		{
			int tmpValue = inputImg.at<uchar>(i,j);
			if (tmpValue != 255)
				valueOfPixels.push_back(tmpValue);
		}
	}

	float histogram[256] = {0};
	for (int i = 0; i < valueOfPixels.size();++i)
	{
		histogram[valueOfPixels[i]]++;
	}


	//normalize histogram
	for(int i = 0;i < 256;i++) 
	{
		histogram[i] = histogram[i] / valueOfPixels.size();
	}

	//average pixel value
	float avgValue = 0;
	for(int i = 0;i < 256;i++) 
	{
		avgValue += i * histogram[i];
	}

	int threshold;	
	float maxVariance = 0;
	float w = 0,u = 0;
	for(int i = 0;i < 256;i++) 
	{
		w += histogram[i];
		u += i*histogram[i];

		float t = avgValue * w - u;
		float variance=t * t / (w * (1 - w));
		if(variance>maxVariance) 
		{
			maxVariance=variance;
			threshold = i;
		}
	}
	//std::cout << threshold << endl;
	cv::threshold(inputImg,outputImg,threshold,255,cv::THRESH_BINARY);
	//cv::imshow("otsu",inputImg);
}

std::vector<std::pair<int,int> > 
	DialImgProcessor::storeStripePoints(cv::Mat inputImg,std::vector<double> outPara,std::vector<double> innerPara,posType pos)
{
	std::vector<std::pair<int,int> > resultVec;
	if (pos == up || pos == total)
	{
		for (int i = inputImg.rows / 2; i < inputImg.rows; ++i)
		{
			for (int j = 0;j < inputImg.cols;++j)
			{
				double judeValue_in = innerPara[0] * i * i + innerPara[1] * i * j + innerPara[2] * j * j 
					+ innerPara[3] * i + innerPara[4] * j + innerPara[5];

				double judeValue_out = outPara[0] * i * i + outPara[1] * i * j + outPara[2] * j * j 
					+ outPara[3] * i + outPara[4] * j + outPara[5];

				if (judeValue_in > 0 && judeValue_out < 0)
				{
					resultVec.push_back(std::pair<int,int>(i,j));
				}
			}

		}
	}
	else
	{
		int height = inputImg.rows;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0;j < inputImg.cols; ++j)
			{
				double judeValue_in = innerPara[0] * (i + height) * (i + height) + innerPara[1] * (i + height) * j + innerPara[2] * j * j 
					+ innerPara[3] * (i + height) + innerPara[4] * j + innerPara[5];

				double judeValue_out = outPara[0] * (i + height) * (i + height) + outPara[1] * (i + height) * j + outPara[2] * j * j 
					+ outPara[3] * (i + height) + outPara[4] * j + outPara[5];

				if (judeValue_in > 0 && judeValue_out < 0)
				{
					resultVec.push_back(std::pair<int,int>(i,j));
				}
			}
		}
	}

	return resultVec;
}

void DialImgProcessor::threshOfStripOtsu( cv::Mat &inputImg,std::vector<double> outPara,std::vector<double> innerPara,posType pos )
{
	std::vector<std::pair<int,int> > resultVec = storeStripePoints(inputImg,outPara,innerPara,pos);
	float histogram[256] = {0};

	for (int i = 0; i < resultVec.size();++i)
	{
		int x = resultVec[i].first;
		int y = resultVec[i].second;
		int piexlValue = inputImg.at<uchar>(x,y);
		histogram[piexlValue]++;
	}


	//normalize histogram
	for(int i = 0;i < 256;i++) 
	{
		histogram[i] = histogram[i] / resultVec.size();
	}

	//average pixel value
	float avgValue = 0;
	for(int i = 0;i < 256;i++) 
	{
		avgValue += i * histogram[i];
	}

	int threshold;	
	float maxVariance = 0;
	float w = 0,u = 0;
	for(int i = 0;i < 256;i++) 
	{
		w += histogram[i];
		u += i*histogram[i];

		float t = avgValue * w - u;
		float variance=t * t / (w * (1 - w));
		if(variance>maxVariance) 
		{
			maxVariance=variance;
			threshold = i;
		}
	}
	cv::threshold(inputImg,inputImg,0.8 * threshold,255,cv::THRESH_BINARY);


}

cv::Point DialImgProcessor::calcElipseCenter( std::vector<double> parameter )
{
	double a = parameter[0];
	double b = parameter[1];
	double c = parameter[2];
	double d = parameter[3];
	double e = parameter[4];

	//(xc,yc)��Բ��λ��
	double xc = (b * e - 2 * c * d) / (4 * a * c - b * b);
	double yc = (b * d- 2 * a * e) / (4 * a * c - b * b);
	cv::Point result = cv::Point(xc,yc);
	return result;
}

double DialImgProcessor::calcSlope( cv::Point point0,cv::Point point1 )
{
	double x0 = point0.y;
	double y0 = point0.x;

	double x1 = point1.y;
	double y1 = point1.x;
	double slope = (x1 - x0) / ( y1 - y0);

	return slope;
}

bool DialImgProcessor::whetherOnLine( double slope,cv::Point routPoint,cv::Point testPoint )
{
	double x0 = routPoint.y;
	double y0 = routPoint.x;
	double xt = testPoint.y;
	double yt = testPoint.x;

	double estimate_x = x0 + slope * (yt - y0);
	if (abs(xt - estimate_x) < 0.5)
	{
		return true;
	}
	return false;
}

double DialImgProcessor::calcElipseRotation( std::vector<double> parameter )
{
	double a = parameter[0];
	double b = parameter[1];
	double c = parameter[2];
	double d = parameter[3];
	double e = parameter[4];
	double stretchCoeff = (a + c + sqrt((a - c) *(a - c) + b * b))
						  /((a + c - sqrt((a - c) *(a - c) + b * b)));
	return stretchCoeff;
}

double DialImgProcessor::
	calcFieldAngle( double stretchCoeff,std::pair<int,int> start,std::pair<int,int> end,std::pair<int,int> center )
{

	start.second = center.second - (center.second - start.second) * stretchCoeff;
	end.second = center.second - (center.second - end.second) * stretchCoeff;

	std::pair<int,int> startVec,endVec;
	startVec.first = start.first - center.first;
	startVec.second = start.second - center.second;
	endVec.first = end.first - center.first;
	endVec.second = end.second - center.second;

	//std::cout << start.first << " " << start.second << std::endl;
	//std::cout << end.first << " " << end.second << std::endl;
	//std::cout << std::endl;
	//std::cout << startVec.first << " " << startVec.second << std::endl;
	//std::cout << endVec.first << " " << endVec.second << std::endl;

	double modeStart = sqrt(double(startVec.first * startVec.first + startVec.second * startVec.second));
	double modeEnd = sqrt(double(endVec.first * endVec.first + endVec.second * endVec.second));

	double product = startVec.first * endVec.first + startVec.second * endVec.second;
	double tmp = product / (modeStart * modeEnd);
	double angle = acos(tmp); //���ص��ǻ���
	angle = angle * 180 / 3.14159;

	double flag = 0;
	// �жϽǶȴ���180����С��180
	flag = endVec.first - (startVec.first / startVec.second) * endVec.first;
	if (flag < 0)
	{
		angle = 360 - angle;
	}

	double result = (angle / (360 - 96)) * 145 - 14.5;
	//std::cout << "angle: " << angle << std::endl;
	//std::cout << "result: " << result << "Kpa" << std::endl;
	return result;
}

void DialImgProcessor::initiImg2BlockProcess( cv::Mat &inputImg,int dividCoeff)
{
	//ֻ�ֳܷ���������������С�������dividCoeff���ڱ�ʾ�ָ�Ļ���

	//��Ƭ
	cv::Rect upArea;
	upArea.x = 0;
	upArea.y = 0;
	upArea.height = inputImg.rows / dividCoeff;
	upArea.width = inputImg.cols;
	cv::Mat upImg = inputImg(upArea);

	cv::Rect downArea;
	downArea.x = 0;
	downArea.y = upImg.rows ;
	downArea.height = inputImg.rows - upImg.rows;
	downArea.width = inputImg.cols;
	cv::Mat downImg = inputImg(downArea);

	//cv::imshow("upImg",upImg);
	//cv::imshow("downImg",downImg);

	//��Ƭ�ָ�
	cv::Mat upSeg;
	cv::Mat downSeg;
	SegmentContext findTarget(otsu);
	findTarget.getTarget(upImg,upSeg);
	findTarget.getTarget(downImg,downSeg);

	//imshow("upSeg",upSeg);
	//imshow("downSeg",downSeg);

	//ƴ��
	upSeg.copyTo(cv::Mat(inputImg,cv::Rect(0,0,upSeg.cols,upSeg.rows)));
	downSeg.copyTo(cv::Mat(inputImg,cv::Rect(0,upImg.rows,downSeg.cols,downSeg.rows)));
	//imshow("segResult",segResult);
}

void DialImgProcessor::dialImg2BlockProcess( cv::Mat &inputImg,std::vector<double> elipseCoeff,int dividCoeff )
{
	cv::Rect upArea;
	upArea.x = 0;
	upArea.y = 0;
	upArea.height = inputImg.rows / dividCoeff;
	upArea.width = inputImg.cols;
	cv::Mat upImg = inputImg(upArea);

	cv::Rect downArea;
	downArea.x = 0;
	downArea.y = upImg.rows ;
	downArea.height = inputImg.rows - upImg.rows;
	downArea.width = inputImg.cols;
	cv::Mat downImg = inputImg(downArea);
	//��ֵ�ָ�
	threshOfElipseOtsu( upImg,elipseCoeff,up);
	threshOfElipseOtsu( downImg,elipseCoeff,down);
	//cv::imshow("halfUpImg",halfUpImg);
	//cv::imshow("halfDownImg",halfDownImg);
	//ƴ��
	upImg.copyTo(cv::Mat(inputImg,cv::Rect(0,0,upImg.cols,upImg.rows)));
	downImg.copyTo(cv::Mat(inputImg,cv::Rect(0,upImg.rows,upImg.cols,downImg.rows)));
}

void DialImgProcessor::dialStripe2BlockProcess( cv::Mat &inputImg,std::vector<double> outElipseCoeff,std::vector<double> inElipseCoeff,int dividCoeff )
{
	cv::Mat upImg,downImg;
	cv::Rect upArea,downArea;

	upArea.x = 0;
	upArea.y = 0;
	upArea.height = inputImg.rows / 2;
	upArea.width = inputImg.cols;
	upImg = inputImg(upArea);

	downArea.x = 0;
	downArea.y = upImg.rows ;
	downArea.height = inputImg.rows - upImg.rows;
	downArea.width = inputImg.cols;
	downImg = inputImg(downArea);
	//imshow("dialUpImg",dialUpImg);
	//imshow("dialDownImg",dialDownImg);

	//��Ƭ������ֵ�ָ����Ϊ���²��ֹ��ղ�����
	cv::Mat upSeg = upImg.clone();
	cv::Mat downSeg = downImg.clone();
	threshOfStripOtsu(upSeg, outElipseCoeff,inElipseCoeff,up);
	threshOfStripOtsu(downSeg, outElipseCoeff,inElipseCoeff,down);
	//imshow("dialUpSeg",dialUpSeg);
	//imshow("dialDownSeg",dialDownSeg);

	//ƴ��
	upSeg.copyTo(cv::Mat(inputImg,cv::Rect(0,0,upSeg.cols,upSeg.rows)));
	downSeg.copyTo(cv::Mat(inputImg,cv::Rect(0,upSeg.rows,upSeg.cols,downSeg.rows)));

	//imshow("dialSeg",dialSeg);
}

bool DialImgProcessor::fussDecision( const cv::Mat &inputImg,int *degreeOfFuss )
{
	cv::Mat grad_H,grad_V,gray_T;
	gray_T = cv::Mat(inputImg.size(),CV_32F,cv::Scalar(0));

	cv::Sobel(inputImg,grad_H,inputImg.depth(),1,0,3,1,cv::BORDER_DEFAULT);
	cv::Sobel(inputImg,grad_V,inputImg.depth(),0,1,3,1,cv::BORDER_DEFAULT);

	int height = inputImg.rows;
	int width = inputImg.cols;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0;j < width; ++j)
		{
			int grad_x = grad_H.at<uchar>(i,j);
			int grad_y = grad_V.at<uchar>(i,j);
			gray_T.at<float>(i,j) = sqrt(double(grad_x * grad_x + grad_y * grad_y));
		}
	}


	float maxGrad = 0;
	for (int i = 0;i < height; ++i)
	{
		for (int j = 0;j < width; ++j)
		{
			float gradValue = gray_T.at<float>(i,j);
			if (gradValue > maxGrad)
				maxGrad = gradValue;
		}
	}

	int count = 0;
	for (int i = 0;i < height; ++i)
	{
		for (int j = 0;j < width; ++j)
		{
			float gradValue = gray_T.at<float>(i,j);
			if (gradValue >= maxGrad / 2)
				count++;
		}
	}


	//std::cout << count << std::endl;


	if (count < 900){
		*degreeOfFuss = 0;
		return false;
	}
	if (count >=900 && count < 1300)
	{
		*degreeOfFuss = 1;
		return true;
	}

	*degreeOfFuss = 2;
	return true;
}
