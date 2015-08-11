#pragma once
#include <vector>
#include <functional>

struct TrackedPoint
{
	int currentTimestamp;
	int id;
	std::pair<float, float> position;
};

class PointTracker
{
	std::vector<TrackedPoint> m_points;
	int m_id;
	int m_timestamp;
public:
	PointTracker();
	~PointTracker();

	void track(std::vector<std::pair<float, float>> points);

	const std::vector<TrackedPoint>& points() const;

	std::function<void(const TrackedPoint&)> onTouch;
	std::function<void(const TrackedPoint&)> onMove;
	std::function<void(const TrackedPoint&)> onRelease;
private:
	/*void onTouch(const TrackedPoint& point);
	void onMove(const TrackedPoint& point);
	void onRelease(const TrackedPoint& point);*/
};

