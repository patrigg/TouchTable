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

	void extract(const std::vector<LineSegment>& segments, std::vector<std::pair<short, short>>& centerPoints);
};

