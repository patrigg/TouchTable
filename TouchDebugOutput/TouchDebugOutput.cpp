// TouchDebugOutput.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <tchar.h>
#include "../TouchDetector/DepthDetector.h"
#include "../TouchDetector/ScanLineSegmenter.h"
#include "../TouchDetector/CenterPointExtractor.h"
#include <OpenNI.h>
#include <chrono>


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

const int ThresholdMin = 30;
const int ThresholdMax = 50;
const int MinBlobSize = 40;


int _tmain(int argc, _TCHAR* argv[])
{
	DepthDetector detector(ThresholdMin, ThresholdMax);
	ScanLineSegmenter segmenter;

	OpenNI::initialize();

	Device device;
	if (device.open(ANY_DEVICE) != STATUS_OK)
	{
		std::cout << "could not open any device\r\n";
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
	stream.setMirroringEnabled(true);
	stream.setVideoMode(mode);
	stream.start();

	std::cout << "press any key to capture background\r\n";
	std::cin.get();

	VideoFrameRef frame;
	stream.readFrame(&frame);

	DepthImage image(320, 240);
	copyFrameToImage(frame, image);

	detector.background(image);

	std::cout << "starting capture loop\r\n";

	CenterPointExtractor centerPointExtractor(MinBlobSize);
	std::chrono::high_resolution_clock timer;
	auto startTime = timer.now();
	int frameId = 0;
	while (true)
	{
		stream.readFrame(&frame);

		copyFrameToImage(frame, image);

		detector.detect(image);

		std::vector<LineSegment> segments;
		segmenter.segment(detector.mask(), segments);


		std::vector<std::pair<float, float>> centerPoints;
		centerPointExtractor.extract(segments, centerPoints);

		if (centerPoints.size())
		{
			std::cout << "point count: " << centerPoints.size();
		
			std::cout << "\t points: ";
		
			for (auto& point : centerPoints)
			{
				std::cout << "(" << point.first << ", " << point.second << ")  ";
			}
			std::cout << "\r\n";
		}

		++frameId;

		
		
		if (frameId % 64 == 0)
		{
			auto stopTime = timer.now();
			
			auto elapsedTime = stopTime - startTime;
			auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();

			std::cout << "\t total frames: " << frameId << "\t fps: " << elapsedMilliseconds / 64 << std::endl;

			startTime = stopTime;
		}
		
	}
	
	openni::OpenNI::shutdown();
	return 0;
}

