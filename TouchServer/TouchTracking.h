#pragma once
#include "../TouchDetector/DepthDetector.h"
#include "../TouchDetector/ScanLineSegmenter.h"
#include "../TouchDetector/CenterPointExtractor.h"
#include "../TouchDetector/PointTracker.h"
#include <OpenNI.h>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <thread>
#include <atomic>
#include <mutex>

struct TouchEvent;

class TouchTracking
{
	openni::VideoStream stream;
	DepthDetector detector;
	ScanLineSegmenter segmenter;
	openni::Device device;

	PointTracker tracker;

	CenterPointExtractor centerPointExtractor;
	int frameId;

	std::vector<LineSegment> segments;
	std::vector<std::pair<float, float>> centerPoints;
	openni::VideoFrameRef frame;
	DepthImage image;
	std::thread worker;
	std::mutex m;

	std::vector<openni::VideoMode> m_videoModes;
	std::atomic<bool> isRunning = false;

	void initializeVideo();
	void run();
	void runSingle();
	bool hFlipped = false;
	bool vFlipped = false;

	TrackedPoint flip(TrackedPoint p);

public:
	void start();
	void stop();

	void videoMode(const openni::VideoMode& mode);

	void removeBackground();

	const std::vector<openni::VideoMode>& videoModes() const;

	void flipHorizontal(bool value);
	bool horizontalFlipped() const;
	void flipVertical(bool value);
	bool verticalFlipped() const;

	TouchTracking();
	~TouchTracking();

	void onTouchEvent(const std::function<void(const TouchEvent&)>& handler);
};

