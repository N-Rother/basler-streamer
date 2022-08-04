#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std::chrono;
int main()
{
	// The sink caps for the 'rtpjpegdepay' need to match the src caps of the 'rtpjpegpay' of the sender pipeline
	// Added 'videoconvert' at the end to convert the images into proper format for appsink, without
	// 'videoconvert' the receiver will not read the frames, even though 'videoconvert' is not present
	// in the original working pipeline

	VideoCapture cap("udpsrc port=5000 caps = \"application/x-rtp, payload=127\" ! rtph264depay ! avdec_h264 ! autovideoconvert ! appsink sync=false",CAP_GSTREAMER);

	if(!cap.isOpened())
	{
		std::cout<<"VideoCapture not opened"<<std::endl;
		exit(-1);
	}

	namedWindow("Receiver", WINDOW_NORMAL);

	Mat frame;
	long double pt;
	long double nt;

	while(true) {
		nt = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
		auto fps = 1.0 / ((nt - pt) / 1000.0);
		pt = nt;
		cap.read(frame);

		if(frame.empty())
			break;

		putText(frame, std::to_string(fps), Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 255), 2);

		imshow("Receiver", frame);
		if(waitKey(1) == 'q')
			break;
	}
	destroyWindow("Receiver");
}