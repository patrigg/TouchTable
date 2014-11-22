#pragma once
#include <vector>

struct Motion
{
	int currentTimestamp;
	int id;
	std::vector<std::pair<double_t, double_t>> points;
};

class MotionRecorder
{
	std::vector<Motion> m_motions;
	int m_id;
	int m_timestamp;
public:
	MotionRecorder();
	~MotionRecorder();

	void track(std::vector<std::pair<double_t, double_t>> points);
	void onTouch(const Motion& motion);
	void onRelease(const Motion& motion);
	const std::vector<Motion>& motions() const;
};

