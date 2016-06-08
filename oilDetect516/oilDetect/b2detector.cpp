// ver 1.0.0

#include "b2detector.h"

using namespace cv;

template<typename T> bool isfinite_c(T arg)
{
	return arg == arg &&
		arg != std::numeric_limits<T>::infinity() &&
		arg != -std::numeric_limits<T>::infinity();
}

void b2detector::operator()(Mat& imgin, Rect& rectout)
{
	Mat imgroi, cannyBW, imgBW;
	Size imgsize(imgin.size());
	Rect Roi(550, 0, 200, 720); // fixed ROI for TEST ONLY
	Size E_k_size(2, 2);

	equalizeHist(imgin(Roi), imgroi);
	blur(imgroi, imgroi, Size(3, 3));
	Canny(imgroi, cannyBW, 90, 270, 3);

	double lv = autothresh(imgroi);
	threshold(imgroi, imgBW, lv, 255, THRESH_BINARY_INV);
	imgBW = imgBW - cannyBW;

	Mat E_kenel = getStructuringElement(MORPH_ELLIPSE, E_k_size);
	erode(imgBW, imgBW, E_kenel);

	rectout = BWanalyze(imgBW, Point(Roi.x,Roi.y)); // caution: this will overwrite imgBW !

	// slight adjust
	rectout = Rect(max(0, rectout.x - E_k_size.width / 2),
		max(0, rectout.y - E_k_size.height / 2),
		min(imgsize.width - rectout.x, rectout.width + E_k_size.width),
		min(imgsize.height - rectout.y, rectout.height + E_k_size.height));
}

double b2detector::autothresh(Mat& imgin)
{
	// calc hist
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	Mat counts;
	calcHist(&imgin, 1, 0, Mat(), counts, 1, &histSize, &histRange);
	
	// calc threshold
	Mat p = Mat::zeros(counts.size(), CV_32F);
	Scalar counts_sum = sum(counts);
	for (int i = 0; i < counts.rows; i++)
	{
		p.at<float>(i, 0) = counts.at<float>(i, 0) / (float)counts_sum[0];
	}
	Mat omega = cumsum(p);
	Mat mu_d = Mat::zeros(counts.size(), CV_32F);
	for (int i = 1; i <= histSize; i++)
	{
		mu_d.at<float>(i - 1, 0) = p.at<float>(i - 1, 0)*(float)i;
	}
	Mat mu = cumsum(mu_d);
	float mu_t = mu.at<float>(histSize - 1, 0);

	Mat sigma_b_squared = Mat::zeros(counts.size(), CV_32F);;
	float maxval = 0;
	for (int i = 0; i < counts.rows; i++)
	{
		sigma_b_squared.at<float>(i, 0) = pow((mu_t*omega.at<float>(i, 0) - mu.at<float>(i, 0)), 2) / (omega.at<float>(i, 0)*(1 - omega.at<float>(i, 0)));
		maxval = sigma_b_squared.at<float>(i, 0) > maxval ? sigma_b_squared.at<float>(i, 0) : maxval;
	}

	double lv = 0, idx = 0, idx_c = 0;
	if (isfinite_c(maxval))
	{
		for (int i = 0; i < counts.rows; i++)
		{
			if (sigma_b_squared.at<float>(i, 0) == maxval)
			{
				idx += i;
				idx_c++;
			}
		}
		idx /= idx_c;
		//lv = idx / (histSize - 1);
	}

	// return idx or lv(normalized)
	return idx;
}

Mat b2detector::cumsum(Mat& arrin)
{
	Mat arrout = arrin.clone();
	for (int i = 1; i < arrin.rows; i++)
	{
		arrout.at<float>(i, 0) += arrout.at<float>(i - 1, 0);
	}
	return arrout;
}

Rect b2detector::BWanalyze(Mat& BWin, Point offsets)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(BWin, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, offsets);

	Mat drawing = Mat::zeros(720, 1280, CV_8U);
	

	vector<vector<Point>> hull(contours.size());
	vector<double> hullArea;
	vector<Rect> boundRect;

	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(drawing, contours, i, Scalar(255, 255, 255));
		convexHull(contours[i], hull[i]);
		hullArea.push_back(contourArea(hull[i]));
		boundRect.push_back(boundingRect(contours[i]));
		if ((contourArea(contours[i]) / hullArea[i] < 0.8) 
			|| (hullArea[i] < 15000) 
			|| (boundRect[i].width/boundRect[i].height > 0.2)) // value for TEST ONLY
		{
			contours.erase(contours.begin() + i);
			hull.erase(hull.begin() + i);
			hullArea.pop_back();
			boundRect.pop_back();
			i--;
		}
	}

	if (hullArea.size() > 0)
	{
		int idx = 0; double maxArea = hullArea[0];
		for (size_t i = 1; i < hullArea.size(); i++)
		{
			if (hullArea[i]>maxArea)
			{
				maxArea = hullArea[i];
				idx = i;
			}
		}
		return boundRect[idx];
	}
	else
		return Rect(0, 0, 0, 0);
}