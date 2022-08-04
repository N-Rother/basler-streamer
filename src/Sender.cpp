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

	VideoWriter out("appsrc ! videoconvert ! video/x-raw,format=BGRx ! v4l2h264enc ! video/x-h264,level=(string)4 ! rtph264pay ! udpsink host=192.168.0.10 port=5000",
					CAP_GSTREAMER,0,30,Size(1920,1080),true
					);

	if(!out.isOpened())
	{
		std::cout<<"VideoCapture or VideoWriter not opened"<<std::endl;
		exit(-1);
	}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
	while(true)
	{
		curFrame = bc.getCapturedImage();

		if(curFrame.data == nullptr)
		{
			std::cout << "image acquisition failed" << std::endl;
			continue;
		}
		out.write(curFrame);
	}
#pragma clang diagnostic pop
}
