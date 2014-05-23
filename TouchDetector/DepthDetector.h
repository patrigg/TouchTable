#pragma once
#include "Image.h"

class DepthDetector
{
public:
	DepthDetector(int width, int height);
	DepthDetector(int width, int height, int threshold_min, int thresholdMax);
	~DepthDetector();

	void background(DepthImage image);
	void detect(const DepthImage& image);

	const DepthImage& background();
	const Image<int>& difference();
	const Image<uint8_t>& mask();
private:
	int m_width;
	int m_height;

	DepthImage m_background;
	Image<int> m_difference;
	Image<uint8_t> m_mask;

	int m_thresholdMin;// = 50;//30;
	int m_thresholdMax;// = 400;// 120;//80;
};

