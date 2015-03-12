#include "PointTracker.h"
#include <algorithm>
#include <iterator>


PointTracker::PointTracker()
	: m_id(0), m_timestamp(0)
{
}


PointTracker::~PointTracker()
{
}

void PointTracker::track(std::vector<std::pair<double_t, double_t>> points)
{
	std::vector<TrackedPoint> currentPoints;
	currentPoints.reserve(points.size());

	for (const auto& p : m_points)
	{
		if (points.empty())
			break;

		std::vector<int> distances;
		
		auto& base = p.position;
		std::transform(begin(points), end(points), std::back_inserter(distances), [&base](const std::pair<double_t, double_t>& p)
		{
			auto diffx = p.first - base.first;
			auto diffy = p.second - base.second;

			return diffx * diffx + diffy * diffy;
		});

		auto min = std::min_element(begin(distances), end(distances));

		if (min != end(distances) && *min < 100)
		{
			auto minPoint = begin(points) + (min - begin(distances));

			TrackedPoint point = {
				m_timestamp,
				p.id,
				*minPoint
			};
			
			currentPoints.push_back(point);

			points.erase(minPoint);
		}
		else
		{
			if (onRelease)
			{
				onRelease(p);
			}
			
		}
	}

	if (onMove)
	{
		for (const auto& p : currentPoints)
		{
			onMove(p);
		}
	}

	m_points = std::move(currentPoints);
	std::transform(begin(points), end(points), std::back_inserter(m_points), [this](const std::pair<double_t, double_t>& p){
		TrackedPoint t { m_id++, m_timestamp, { p } };
		if (onTouch)
		{
			onTouch(t);
		}
		
		return t;
	});

	++m_timestamp;
}

const std::vector<TrackedPoint>& PointTracker::points() const
{
	return m_points;
}
/*
void PointTracker::onTouch(const TrackedPoint& point)
{

}

void PointTracker::onMove(const TrackedPoint& point)
{

}

void PointTracker::onRelease(const TrackedPoint& point)
{

}
*/