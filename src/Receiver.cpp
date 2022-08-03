#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace cv;
int main()
{
	// The sink caps for the 'rtpjpegdepay' need to match the src caps of the 'rtpjpegpay' of the sender pipeline
	// Added 'videoconvert' at the end to convert the images into proper format for appsink, without
	// 'videoconvert' the receiver will not read the frames, even though 'videoconvert' is not present
	// in the original working pipeline
	VideoCapture cap("udpsrc port=5000 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink",CAP_GSTREAMER);

	if(!cap.isOpened())
	{
		std::cout<<"VideoCapture not opened"<<std::endl;
		exit(-1);
	}

	namedWindow("Receiver", WINDOW_NORMAL);

	Mat frame;

	while(true) {

		cap.read(frame);

		if(frame.empty())
			break;

		imshow("Receiver", frame);
		if(waitKey(1) == 'q')
			break;
	}
	destroyWindow("Receiver");
}