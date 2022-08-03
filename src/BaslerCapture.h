#ifndef PRAKTIKUM_2_BASLERCAPTURE_HPP
#define PRAKTIKUM_2_BASLERCAPTURE_HPP

#include <pylon/PylonIncludes.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <thread>


using namespace Pylon;
using namespace cv;
class BaslerCapture
{
	int frameWidth;
	int frameHeight;
	CInstantCamera* camera;
	CGrabResultPtr ptrGrabResult;
	CPylonImage pylonImage;
	CImageFormatConverter* formatConverter;
public:
	BaslerCapture();
	void startCapture();
	Mat getCapturedImage();
};


#endif //PRAKTIKUM_2_BASLERCAPTURE_HPP
