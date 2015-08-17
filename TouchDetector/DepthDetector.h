#pragma once
#include "Image.h"

class DepthDetector
{
public:
	DepthDetector();
	DepthDetector(int threshold_min, int thresholdMax);
	~DepthDetector();

	void background(DepthImage image);
	void detect(const DepthImage& image);

	const DepthImage& background();
	const Image<int>& difference();
	const Image<uint8_t>& mask();
	void threshold(int min, int max);
	void thresholdMin(int min);
	void thresholdMax(int max);

private:
	DepthImage m_background;
	Image<int> m_difference;
	Image<uint8_t> m_mask;

	int m_thresholdMin;
	int m_thresholdMax;
};

