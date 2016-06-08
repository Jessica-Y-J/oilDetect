// version 3.1.3c
// 20160504

#include "Cdetector_ver3.h"

void Cdetector3::rstoptm(vector<Vec3f> rst, vector<Rect>& rects, Size sz)
{
	int r_max = rmax(rst);
	rects.clear();

	for (size_t i = 0; i < rst.size(); i++)
	{
		Point center(cvRound(rst[i][0]), cvRound(rst[i][1]));
		rects.push_back(Rect(max(center.x - r_max,0), max(center.y - r_max,0), min(r_max * 2,(sz.width-center.x)*2), min(r_max * 2,(sz.height-center.y)*2)));
	}
}

int Cdetector3::rmax(vector<Vec3f> rst)
{
	int r_max = 0;
	for (size_t i = 0; i < rst.size(); i++)
	{
		r_max = max(cvRound(rst[i][2]), r_max);
	}
	return r_max;
}

bool Cdetector3::chkal(vector<Vec3f>& rst)
{
	bool rt = false;
	vector<Vec3f> temp;
	int r_max = rmax(rst);

	for (size_t i = 0; i < rst.size() - 1; i++)
	{
		for (size_t j = i + 1; j < rst.size(); j++)
		{
			if ((abs(rst[i][0] - rst[j][0]) < ((float)r_max / 4)) && (abs(rst[i][1] - rst[j][1]) > ((float)r_max * 1.8)))
			{
				rt = true;
				temp.push_back(rst[i]);
				temp.push_back(rst[j]);
				break;
			}
		}
		if (rt) break;
	}

	if (rt)
	{
		rst.swap(temp);
	}

	return rt;
}

void Cdetector3::rating(vector<Vec3f>& rst, int& rt, double& ns)
{
	if (rst.size() > 8){rt = -1;return;}

	double def_step = -6;

	switch (rst.size())
	{
	case 0:
		rt = 0;
		if (ns == 0) ns = def_step;
		else if (ns > 0) ns /= 2;
		else ns *= 2;
		break;

	case 1:
		rt = 1;
		if (ns == 0) ns = def_step;
		else if (ns > 0) ns /= 2;
		else ns *= 2;	
		break;

	case 2:
	case 3:
		if (chkal(rst)){ rt = 2; break; }

	default:
		rt = 0;
		if (ns == 0) ns = def_step;
		else if (ns > 0) ns *= 2;
		else ns /= 2;
		break;
	}
}

void Cdetector3::operator()(Mat& image, vector<Rect>& rects)
{
	double defc = 50, defa = 50, ns = 0;
	vector<Vec3f> temp1, temp2, temp3, rst;
	int rt[3], tempr = 0;

	GaussianBlur(image, image, Size(9, 9), 2, 2);
	HoughCircles(image, temp1, CV_HOUGH_GRADIENT, 1, image.rows / 8, defc, defa, 30, 180);
	rating(temp1, rt[0], ns);

	if (rt[0] != 2)
	{
		if (rt[0] == 1) 
		{ 
			rst = temp1; 
			tempr = rmax(rst);
		}
		HoughCircles(image, temp2, CV_HOUGH_GRADIENT, 1, image.rows / 8, defc + ns, defa + ns, 30, 180);
		rating(temp2, rt[1], ns);

		if (rt[1] != 2)
		{

			if ((rt[1] == 1) && (rmax(temp2) > tempr))
			{
				rst = temp2;
				tempr = rmax(rst);
			}
			HoughCircles(image, temp3, CV_HOUGH_GRADIENT, 1, image.rows / 8, defc + ns, defa + ns, 30, 180);
			rating(temp3, rt[2], ns);
			if ((rt[2] == 1) && (rmax(temp3) > tempr))
			{
				rst = temp3;
				tempr = rmax(rst);
			}
			if (rt[2] == 2) rst = temp3;
		}
		else rst = temp2;
	}
	else rst = temp1;

	rstoptm(rst, rects, image.size());
}