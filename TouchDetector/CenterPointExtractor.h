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

	void minBlobSize(short size);

	void extract(const std::vector<LineSegment>& segments, std::vector<std::pair<float, float>>& centerPoints);
};

