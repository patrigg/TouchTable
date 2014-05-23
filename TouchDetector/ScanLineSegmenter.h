#pragma once
#include <cstdint>
#include <vector>
#include "LineSegment.h"

template<typename T>
class Image;

class ScanLineSegmenter
{
public:
	void segment(const Image<uint8_t>& image, std::vector<LineSegment> & segmented);
};

