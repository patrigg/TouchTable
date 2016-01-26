#include "TouchTracking.h"
#include <iostream>
#include <chrono>
#include "TouchEvent.h"

using namespace openni;

namespace {
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
}

const std::vector<VideoMode>& TouchTracking::videoModes() const
{
	return m_videoModes;
}

TouchTracking::TouchTracking()
{
	initializeVideo();
}

void TouchTracking::onTouchEvent(const std::function<void(const TouchEvent&)>& handler)
{
	std::lock_guard<std::mutex> lock(m);
	tracker.onTouch = [handler, this](const TrackedPoint& p){ handler({ frameTime, TouchEvent::Touch, flip(p) }); };
	tracker.onMove = [handler, this](const TrackedPoint& p){ handler({ frameTime, TouchEvent::Move, flip(p) }); };
	tracker.onRelease = [handler, this](const TrackedPoint& p){ handler({ frameTime, TouchEvent::Release, flip(p) }); };
}

TrackedPoint TouchTracking::flip(TrackedPoint p)
{
	if (hFlipped)
	{
		p.position.first = image.width() - p.position.first;
	}
	if (vFlipped)
	{
		p.position.second = image.height() - p.position.second;
	}
	return p;
}

void TouchTracking::initializeVideo()
{
	OpenNI::initialize();

	if (device.open(ANY_DEVICE) != STATUS_OK)
	{
		throw std::runtime_error("could not open any device!");
	}

	if (!device.hasSensor(SENSOR_DEPTH))
	{
		throw std::runtime_error("sensor cannot receive depth!");
	}

	auto info = device.getSensorInfo(SENSOR_DEPTH);
	auto& modes = info->getSupportedVideoModes();
	//std::cout << "depth sensor supported modes:\r\n";
	for (int i = 0; i < modes.getSize(); ++i)
	{
		m_videoModes.push_back(modes[i]);
		//std::cout << "pixel format: " << mode.getPixelFormat() << "\t with: " << mode.getResolutionX() << "x" << mode.getResolutionY() << "@" << mode.getFps() << " fps\r\n";
	}

	VideoMode mode;
	mode.setFps(60);
	mode.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
	mode.setResolution(320, 240);
	videoMode(mode);

	stream.setMirroringEnabled(false);
}

void TouchTracking::videoMode(const VideoMode& mode)
{
	if (isRunning)
	{
		stop();
		stream.setVideoMode(mode);
		start();
	}
	else
	{
		stream.setVideoMode(mode);
	}
}


TouchTracking::~TouchTracking()
{
	stream.stop();
	openni::OpenNI::shutdown();
}


void TouchTracking::start()
{
	stream.create(device, SENSOR_DEPTH);
	
	if (stream.start() == STATUS_OK)
	{
		frameId = 0;
		isRunning = true;
		worker = std::thread([this](){ run(); });
	}
}

void TouchTracking::stop()
{
	isRunning = false;
	worker.join();

	stream.stop();
}

bool TouchTracking::running() const
{
	return isRunning;
}

void TouchTracking::run()
{
	while (isRunning)
	{
		auto time = std::chrono::system_clock::now();
		std::lock_guard<std::mutex> lock(m);
		runSingle();
		if (m_delay > 0)
		{
			std::this_thread::sleep_until(time + std::chrono::milliseconds(m_delay));
		}
	}
}

void TouchTracking::removeBackground()
{
	std::lock_guard<std::mutex> lock(m);

	if (isRunning)
	{
		stream.readFrame(&frame);

		copyFrameToImage(frame, image);

		detector.background(image);
	}
}

void TouchTracking::runSingle()
{
	stream.readFrame(&frame);

	frameTime = std::chrono::system_clock::now();

	copyFrameToImage(frame, image);

	detector.detect(image);

	segmenter.segment(detector.mask(), segments);

	centerPointExtractor.extract(segments, centerPoints);

	tracker.track(centerPoints);
}

void TouchTracking::flipHorizontal(bool value)
{
	std::lock_guard<std::mutex> lock(m);
	hFlipped = value;
}

bool TouchTracking::horizontalFlipped() const
{
	return hFlipped;
}

void TouchTracking::flipVertical(bool value)
{
	std::lock_guard<std::mutex> lock(m);
	vFlipped = value;
}

bool TouchTracking::verticalFlipped() const
{
	return vFlipped;
}

short TouchTracking::minBlobSize() const
{
	return centerPointExtractor.minBlobSize();
}

void TouchTracking::minBlobSize(short size)
{
	std::lock_guard<std::mutex> lock(m);
	centerPointExtractor.minBlobSize(size);
}

void TouchTracking::depthThresholdMin(int min)
{
	std::lock_guard<std::mutex> lock(m);
	detector.thresholdMin(min);
}

void TouchTracking::depthThresholdMax(int max)
{
	std::lock_guard<std::mutex> lock(m);
	detector.thresholdMax(max);
}

void TouchTracking::depthThreshold(int min, int max)
{
	std::lock_guard<std::mutex> lock(m);
	detector.threshold(min, max);
}

int TouchTracking::depthThresholdMin() const
{
	return detector.thresholdMin();
}

int TouchTracking::depthThresholdMax() const
{
	return detector.thresholdMax();
}

int TouchTracking::delay() const
{
	return m_delay;
}

void TouchTracking::delay(int delay)
{
	std::lock_guard<std::mutex> lock(m);
	m_delay = delay;
}