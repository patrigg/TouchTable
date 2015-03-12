// TouchDebugOutput.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <sstream>
#include <tchar.h>
#include "../TouchDetector/DepthDetector.h"
#include "../TouchDetector/ScanLineSegmenter.h"
#include "../TouchDetector/CenterPointExtractor.h"
#include "../TouchDetector/PointTracker.h"
#include <OpenNI.h>
#include <chrono>
#include "UdpSender.h"
#include <algorithm>
#include <numeric>


using namespace openni;

void copyFrameToImage(VideoFrameRef frame, DepthImage& image)
{
	if (image.height() != frame.getHeight() || image.width() != frame.getWidth())
	{
		image = DepthImage(
			DepthImage::Data(
			static_cast<const uint16_t*>(frame.getData()),
			static_cast<const uint16_t*>(frame.getData()) + frame.getWidth() * frame.getHeight()),
			frame.getWidth(),
			frame.getHeight());
	}
	else
	{
		image.data(DepthImage::Data(
			static_cast<const uint16_t*>(frame.getData()),
			static_cast<const uint16_t*>(frame.getData()) + frame.getWidth() * frame.getHeight()));
	}
}

//const int ThresholdMin = 20;
//const int ThresholdMax = 70;
//const int MinBlobSize = 40;


const int ThresholdMin = 40; // 30;
const int ThresholdMax = 80;
const int MinBlobSize = 60;

void serializeEvent(std::stringstream& stream, char type, const TrackedPoint& point)
{
	const char b = 1;
	stream.write(&b, sizeof(char));
	stream.write(reinterpret_cast<const char*>(&point.currentTimestamp), sizeof(int));
	stream.write(reinterpret_cast<const char*>(&point.id), sizeof(int));
	stream.write(reinterpret_cast<const char*>(&type), sizeof(char));
	stream.write(reinterpret_cast<const char*>(&point.position.first), sizeof(double_t));
	stream.write(reinterpret_cast<const char*>(&point.position.second), sizeof(double_t));
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		std::cout << "usage:\r\nTouchServer [host] [port]\r\n" << "press a key to continue"; std::cin.get();
		return 1;
	}

	DepthDetector detector(320, 240, ThresholdMin, ThresholdMax);
	ScanLineSegmenter segmenter;

	OpenNI::initialize();

	Device device;
	if (device.open(ANY_DEVICE) != STATUS_OK)
	{
		std::cout << "could not open any device\r\n" << "press a key to continue"; std::cin.get();
		return 1;
	}

	if (device.hasSensor(SENSOR_DEPTH))
	{
		auto info = device.getSensorInfo(SENSOR_DEPTH);
		auto& modes = info->getSupportedVideoModes();
		std::cout << "depth sensor supported modes:\r\n";
		for (int i = 0; i < modes.getSize(); ++i)
		{
			auto& mode = modes[i];
			std::cout << "pixel format: " << mode.getPixelFormat() << "\t with: " << mode.getResolutionX() << "x" << mode.getResolutionY() << "@" << mode.getFps() << " fps\r\n";
		}
	}

	VideoStream stream;
	stream.create(device, SENSOR_DEPTH);
	VideoMode mode;
	mode.setFps(25);
	mode.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
	mode.setResolution(320, 240);
	stream.setMirroringEnabled(false);
	stream.setVideoMode(mode);
	stream.start();

	std::cout << "press any key to capture background\r\n";
	std::cin.get();

	VideoFrameRef frame;
	stream.readFrame(&frame);

	DepthImage image(320, 240);
	copyFrameToImage(frame, image);

	detector.background(image);

	UdpSender sender(argv[1], std::stoi(argv[2]));

	PointTracker tracker;

	tracker.onTouch = [&sender](const TrackedPoint& p){
		std::stringstream s;
		serializeEvent(s, 1, p);
		sender.send(s.str());
		/*std::stringstream s;
		s << "Touched at: " << p.currentTimestamp << " id: " << p.id << " x: " << p.position.first << " y: " << p.position.second;
		std::cout << s.str() << "\r\n";
		sender.send(s.str());*/
	};

	tracker.onMove = [&sender](const TrackedPoint& p){
		std::stringstream s;
		serializeEvent(s, 2, p);
		sender.send(s.str());
		//s << "Moved at: " << p.currentTimestamp << " id: " << p.id << " x: " << p.position.first << " y: " << p.position.second;
		
		//std::cout << s.str() << "\r\n";
	};

	tracker.onRelease = [&sender](const TrackedPoint& p){
		std::stringstream s;
		serializeEvent(s, 3, p);
		sender.send(s.str());
		/*std::stringstream s;
		s << "Released at: " << p.currentTimestamp << " id: " << p.id << " x: " << p.position.first << " y: " << p.position.second;
		std::cout << s.str() << "\r\n";
		sender.send(s.str());*/
	};

	std::cout << "starting capture loop\r\n";

	

	CenterPointExtractor centerPointExtractor(MinBlobSize);
	int frameId = 0;
	while (true)
	{
		stream.readFrame(&frame);

		copyFrameToImage(frame, image);

		detector.detect(image);

		std::vector<LineSegment> segments;
		segmenter.segment(detector.mask(), segments);


		std::vector<std::pair<double_t, double_t>> centerPoints;
		centerPointExtractor.extract(segments, centerPoints);

		if (centerPoints.size() > 0) {
			std::vector<uint16_t> depthValues(centerPoints.size());
			int i;
			i = 3;

			std::transform(
				begin(centerPoints), end(centerPoints),
				begin(depthValues),
				//lambda, das image in den scope erh�lt
				[&image](std::pair<double_t, double_t> point)
			{
				//image.data() liefert ein array f�r das gesamte Bild zur�ck
				return image.data()[point.first + point.second * image.width()];
			}
			);

			std::cout.precision(2);
			std::cout << "\rcenter point(0) at x: " << centerPoints[0].first << " y: " << centerPoints[0].second << " depth: " << depthValues[0] << "              ";


			std::vector<std::pair<double_t, double_t>> scaledPoints;

			// transformation von bildkoordinaten in weltkoordinaten
			//		scaledPoints.push_back(std::make_pair(3, 4));
		}
		static int ticks = 0;
		if (ticks < 50)
		{
			
			if (centerPoints.size()==1)
			{
				tracker.track(centerPoints);
				ticks = 0;
			}
			else
			{
				++ticks;
			}
		}
		else
		{
			tracker.track(centerPoints);
		}
		
	}

	openni::OpenNI::shutdown();
	return 0;
}

