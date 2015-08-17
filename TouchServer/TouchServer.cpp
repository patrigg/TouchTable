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

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	if (argc < 4 && argc != 2)
	{
		std::cout << "usage:\r\nTouchServer udp [host] [port] | console\r\n";
		return 1;
	}

	boost::asio::io_service io_service;

	std::unique_ptr < ISender > sender;
	//
	std::unique_ptr<IReceiver> receiver;
	
	if (std::string("udp") == argv[1])
	{
		sender = std::make_unique<UdpSender>(io_service, argv[1], std::stoi(argv[2]));
		receiver = std::make_unique<UdpReceiver>(io_service, 14000);
	}
	else if (std::string("console") == argv[1])
	{
		sender = std::make_unique<StreamSender>(std::cout);
		receiver = std::make_unique<StreamReceiver>(std::cin, *sender);
	}
	
	EventSerializer serializer(EventSerializer::Mode::Json);

	TouchTracking tracking;

	tracking.onTouchEvent([&sender, &serializer](const TouchEvent& evt){
		std::stringstream s;
		serializer.serialize(s, evt);
		sender->send(s.str());
	});

	Configurator config(tracking, serializer);
	
	receiver->onReceive = std::bind(&Configurator::receive, config, std::placeholders::_1, std::placeholders::_2);


	std::cout << "press any key to capture background\r\n";
	//std::cin.get();
	
	tracking.start();
	
	//tracking.removeBackground();



	std::cout << "starting capture loop\r\n";

	receiver->run();
	//io_service.run();

	return 0;
}

