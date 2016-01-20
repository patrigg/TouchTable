#include "DepthDetector.h"
#include <algorithm>


DepthDetector::DepthDetector()
: m_thresholdMin(30), m_thresholdMax(80)
{
}

DepthDetector::DepthDetector(int thresholdMin, int thresholdMax)
: m_thresholdMin(thresholdMin), m_thresholdMax(thresholdMax)
{
}

void DepthDetector::threshold(int min, int max)
{
	m_thresholdMin = min;
	m_thresholdMax = max;
}

void DepthDetector::thresholdMin(int min)
{
	m_thresholdMin = min;
}

void DepthDetector::thresholdMax(int max)
{
	m_thresholdMax = max;
}

int DepthDetector::thresholdMin() const
{
	return m_thresholdMin;
}

int DepthDetector::thresholdMax() const
{
	return m_thresholdMax;
}

DepthDetector::~DepthDetector()
{
}

void DepthDetector::background(DepthImage image)
{
	m_background = std::move(image);
	m_difference = Image<int>(image.width(), image.height());
	m_mask = Image<uint8_t>(image.width(), image.height());
}

void DepthDetector::detect(const DepthImage& image)
{
	if (m_background.width() != image.width() || m_background.height() != image.height())
	{
		background(image);
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