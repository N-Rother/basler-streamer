#include "BaslerCapture.h"

BaslerCapture::BaslerCapture()
{
	try
	{
		Pylon::CTlFactory& tl_factory = Pylon::CTlFactory::GetInstance();
		camera = new CInstantCamera(tl_factory.CreateFirstDevice());

		std::cout << "Found Camera: " << camera->GetDeviceInfo().GetModelName() << std::endl;

		GenApi::INodeMap &nodemap = camera->GetNodeMap();
		camera->Open();

		GenApi::CIntegerPtr Width = nodemap.GetNode("Width");
		GenApi::CIntegerPtr Height = nodemap.GetNode("Height");
		GenApi::CBooleanPtr CenterX = nodemap.GetNode("CenterX");
		GenApi::CBooleanPtr CenterY = nodemap.GetNode("CenterY");

		GenApi::CFloatPtr Exposure = nodemap.GetNode("ExposureTime");
		GenApi::CEnumerationPtr ExposureAuto = nodemap.GetNode("ExposureAuto");

		CenterX->SetValue(true);
		CenterY->SetValue(true);
		Width->SetValue(1920);
		Height->SetValue(1080);
		Exposure->SetValue(33000);
		ExposureAuto->SetIntValue(0);

		this->frameHeight = (int) Height->GetValue();
		this->frameWidth = (int) Width->GetValue();
		camera->MaxNumBuffer = 5;
		//this->CapturedImage = new Mat(this->frameHeight, this->frameWidth, CV_8UC3);

	}
	catch (const std::exception &e)
	{
		std::cout << "Failed to initialize camera. Are you sure a Basler device is connected?" << std::endl;
	}
}

void BaslerCapture::startCapture()
{
	formatConverter = new CImageFormatConverter();
	ptrGrabResult = CGrabResultPtr();
	pylonImage = CPylonImage();
	formatConverter->OutputPixelFormat = PixelType_BGR8packed;
	//namedWindow("Basler Video", CV_WINDOW_AUTOSIZE);
	VideoWriter cvVideoCreator;
	Size frameSize = Size(frameWidth, frameHeight);
	cvVideoCreator.open("baslerCapture.avi", cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), 30, frameSize, true);
	camera->StartGrabbing(3000, GrabStrategy_LatestImageOnly);
}

Mat BaslerCapture::getCapturedImage()
{
	Mat openCVImage;

	if (camera->IsGrabbing())
	{
		camera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

		if ((ptrGrabResult)->GrabSucceeded())
		{
			formatConverter->Convert(pylonImage, ptrGrabResult);
			openCVImage = Mat((ptrGrabResult)->GetHeight(), (ptrGrabResult)->GetWidth(), CV_8UC3,
							  (uint8_t *) pylonImage.GetBuffer());

			//imshow("Basler Video", openCVImage);
			//openCVImage->copyTo(*(this->CapturedImage));
			//waitKey(1);
		}

	} else
	{
		camera->StartGrabbing(3000, GrabStrategy_LatestImageOnly);
	}
	return openCVImage; //this->CapturedImage;
}

