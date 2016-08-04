#pragma once
#include "ImgProcessor.h"
#include "MetalImgProcessor.h"
#include "CircleImgProcessor.h"
#include "StickImgProcessor.h"
#include "OnticImgProcessor.h"
#include "DialImgProcessor.h"

class ImgProcessorContext
{
public:
	ImgProcessorContext(void);
	ImgProcessorContext(typeofImg type);
	~ImgProcessorContext(void);
	ResultOfDetect getResult(const cv::Mat inputImg);

private:
	ImgProcessor *ImgProcess;
};

