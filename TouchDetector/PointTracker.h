#pragma once
#include <vector>

struct Motion
{
	int currentTimestamp;
	int id;
	std::vector<std::pair<short, short>> points;
};

class PointTracker
{
	std::vector<Motion> m_motions;
	int m_id;
	int m_timestamp;
public:
	PointTracker();
	~PointTracker();

	void track(std::vector<std::pair<short, short>> points);
	void onTouch(const Motion& motion);
	void onRelease(const Motion& motion);
	const std::vector<Motion>& motions() const;
};

