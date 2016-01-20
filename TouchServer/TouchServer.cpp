// TouchDebugOutput.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <sstream>
#include "targetver.h"

#ifdef WIN32
#include <tchar.h>
#endif

#include "UdpSender.h"
#include "UdpReceiver.h"

#include "StreamSender.h"
#include "StreamReceiver.h"

#include "TouchTracking.h"
#include "EventSerializer.h"
#include "TouchEvent.h"
#include "Configurator.h"
#include <memory>

enum class OutputMode
{
	Console,
	Udp
};

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	/*if (argc < 6 && argc != 4)
	{
		std::cout << "usage:\r\nTouchServer udp [host] [port] [hFlip] [vFlip] | console [hFlip] [vFlip]\r\n";
		return 1;
	}*/

	boost::asio::io_service io_service;

	std::unique_ptr < ISender > sender;
	//
	std::unique_ptr<IReceiver> receiver;

	TouchTracking tracking;

	OutputMode outputMode = OutputMode::Console;
	bool stopped = false;

	EventSerializer::Mode serializationMode = EventSerializer::Mode::Binary;
	for (int argIdx = 1; argIdx < argc; ++argIdx)
	{
		if (std::string("horizontal_flip") == argv[argIdx])
		{
			tracking.flipHorizontal(true);
		}
		else if (std::string("vertical_flip") == argv[argIdx])
		{
			tracking.flipVertical(true);
		}
		else if (std::string("binary") == argv[argIdx])
		{
			serializationMode = EventSerializer::Mode::Binary;
		}
		else if (std::string("json") == argv[argIdx])
		{
			serializationMode = EventSerializer::Mode::Json;
		}
		else if (std::string("udp") == argv[argIdx])
		{
			sender = std::make_unique<UdpSender>(io_service, argv[argIdx + 1], std::stoi(argv[argIdx + 2]));
			receiver = std::make_unique<UdpReceiver>(io_service, 14000);
			outputMode = OutputMode::Udp;
			argIdx += 2;
		}
		else if (std::string("console") == argv[argIdx])
		{
			outputMode = OutputMode::Console;
		}
		else if (std::string("min_blob_size") == argv[argIdx])
		{
			++argIdx;
			tracking.minBlobSize(std::stoi(argv[argIdx]));
		}
		else if (std::string("depth_threshold_min") == argv[argIdx])
		{
			++argIdx;
			tracking.depthThresholdMin(std::stoi(argv[argIdx]));
		}
		else if (std::string("depth_threshold_max") == argv[argIdx])
		{
			++argIdx;
			tracking.depthThresholdMax(std::stoi(argv[argIdx]));
		}
		else if (std::string("depth_threshold") == argv[argIdx])
		{
			tracking.depthThreshold(std::stoi(argv[argIdx + 1]), std::stoi(argv[argIdx + 2]));
			argIdx += 2;
		}
		else if (std::string("stopped") == argv[argIdx])
		{
			stopped = true;
		}
		else
		{
			std::cerr << "unrecognized command-line argument: " << argv[argIdx] << "\r\n";
		}
	}

	if (outputMode == OutputMode::Console)
	{
		sender = std::make_unique<StreamSender>(std::cout);
		receiver = std::make_unique<StreamReceiver>(std::cin, *sender);
	}
	
	EventSerializer serializer(serializationMode);


	tracking.onTouchEvent([&sender, &serializer](const TouchEvent& evt){
		std::stringstream s;
		serializer.serialize(s, evt);
		sender->send(s.str());
	});

	Configurator config(tracking, serializer);
	
	receiver->onReceive = std::bind(&Configurator::receive, config, std::placeholders::_1, std::placeholders::_2);


	//std::cout << "press any key to capture background\r\n";
	//std::cin.get();
	
	if (!stopped)
	{
		tracking.start();
	}
	
	//tracking.removeBackground();



	//std::cout << "starting capture loop\r\n";

	receiver->run();
	//io_service.run();

	return 0;
}

