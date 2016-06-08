#include "ImgProcessorContext.h"


ImgProcessorContext::ImgProcessorContext( void )
	:ImgProcess(NULL)
{

}

ImgProcessorContext::ImgProcessorContext( typeofImg type )
	:ImgProcess(NULL)
{
	switch(type)
	{
	case circleImg:
		ImgProcess = new CircleImgProcessor();
		break;
	case metalImg:
		ImgProcess = new MetalImgProcessor();
		break;
	case stickImg:
		ImgProcess = new StickImgProcessor();
		break;
	case onticImg:
		ImgProcess = new OnticImgProcessor();
		break;
	default:
		break;
	}
}


ImgProcessorContext::~ImgProcessorContext(void)
{
	if (ImgProcess != NULL)
	{
		delete ImgProcess;
	}
}

ResultOfDetect ImgProcessorContext::getResult( const cv::Mat inputImg )
{
	return ImgProcess->imgProcessor(inputImg);

}
