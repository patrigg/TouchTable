#pragma once
#include <vector>
#include "LineSegment.h"

class CenterPointExtractor
{
	short m_minBlobSize;
public:
	CenterPointExtractor();
	CenterPointExtractor(short minBlobSize);
	~CenterPointExtractor();

	void extract(const std::vector<LineSegment>& segments, std::vector<std::pair<double_t, double_t>>& centerPoints);
};

