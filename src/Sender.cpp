#include <iostream>
#include "BaslerCapture.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	PylonAutoInitTerm init;
	Mat curFrame, prevFrame;
	Mat modifiedFrame;
	BaslerCapture bc;

	bc.startCapture();
	usleep(100);
	namedWindow("Sender", WINDOW_NORMAL);

	char* pipeline;

	if (argc == 2)
	{
		pipeline = argv[1];
	} else {
		pipeline = (char*)"appsrc ! videoconvert ! video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1 ! videoconvert ! x264enc tune=zerolatency bitrate=16000 speed-preset=ultrafast ! rtph264pay ! udpsink host=127.0.0.1 port=5000";
	}

	VideoWriter out(pipeline,
					CAP_GSTREAMER,0,30,Size(1920,1080),true
					);

	if(!out.isOpened())
	{
		std::cout<<"VideoCapture or VideoWriter not opened"<<std::endl;
		exit(-1);
	}

	while(true)
	{
		curFrame = bc.getCapturedImage();

		if(curFrame.data == nullptr)
		{
			std::cout << "image acquisition failed" << std::endl;
			continue;
		}
		out.write(curFrame);

		//imshow("Sender", curFrame);
		if(waitKey(1) == 'q')
			break;
	}
	destroyWindow("Capture");
}
