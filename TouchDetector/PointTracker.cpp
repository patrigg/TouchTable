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

void PointTracker::track(std::vector<std::pair<short, short>> points)
{
	for (auto& m : m_motions)
	{
		if (points.empty())
			break;

		std::vector<int> distances;
		
		auto& base = m.points.back();
		std::transform(begin(points), end(points), std::back_inserter(distances), [&base](const std::pair<short, short>& p)
		{
			auto diffx = p.first - base.first;
			auto diffy = p.second - base.second;

			return diffx * diffx + diffy * diffy;
		});

		auto min = std::min_element(begin(distances), end(distances));

		if (*min < 100)
		{
			auto minPoint = begin(points) + (min - begin(distances));

			m.currentTimestamp = m_timestamp;
			m.points.push_back(*minPoint);

			points.erase(minPoint);
		}
		else
		{
			onRelease(m);
		}
	}

	m_motions.resize(std::remove_if(begin(m_motions), end(m_motions), [this](const Motion& m){
		return this->m_timestamp != m.currentTimestamp;
	}) - begin(m_motions));
	
	std::transform(begin(points), end(points), std::back_inserter(m_motions), [this](const std::pair<short, short>& p){
		Motion m { m_id++, m_timestamp, { p } };
		onTouch(m);
		return m;
	});

	++m_timestamp;
}

const std::vector<Motion>& PointTracker::motions() const
{
	return m_motions;
}

void PointTracker::onTouch(const Motion& motion)
{

}

void PointTracker::onRelease(const Motion& motion)
{

}