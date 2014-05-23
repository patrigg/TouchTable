#include "DepthDetector.h"
#include <algorithm>


DepthDetector::DepthDetector(int width, int height)
: m_width(width), m_height(height), m_background(width, height), m_mask(width, height), m_difference(width, height), m_thresholdMin(30), m_thresholdMax(80)
{
}

DepthDetector::DepthDetector(int width, int height, int thresholdMin, int thresholdMax)
: m_width(width), m_height(height), m_background(width, height), m_mask(width, height), m_difference(width, height), m_thresholdMin(thresholdMin), m_thresholdMax(thresholdMax)
{
}



DepthDetector::~DepthDetector()
{
}

void DepthDetector::background(DepthImage image)
{
	if (m_width != image.width() || m_height != image.height())
	{
		throw "incompatible image resolutions";
	}

	m_background = std::move(image);
}

void DepthDetector::detect(const DepthImage& image)
{
	if (m_width != image.width() || m_height != image.height())
	{
		throw "incompatible image resolutions";
	}

	std::transform(
		begin(image.data()), end(image.data()),
		begin(m_background.data()),
		begin(m_difference.data()),
		[](uint16_t i, uint16_t b) {
			return b - i;
		});

	std::transform(
		begin(m_difference.data()), end(m_difference.data()),
		begin(m_mask.data()),
		[this](int d){
			if (d > m_thresholdMin && d < m_thresholdMax)
			{
				return 255;
			}
			else
			{
				return 0;
			}
		});
}

const DepthImage& DepthDetector::background()
{
	return m_background;
}
const Image<int>& DepthDetector::difference()
{
	return m_difference;
}
const Image<uint8_t>& DepthDetector::mask()
{
	return m_mask;
}