#include "CenterPointExtractor.h"


CenterPointExtractor::CenterPointExtractor()
	: m_minBlobSize(1)
{
}

CenterPointExtractor::CenterPointExtractor(short minBlobSize)
	: m_minBlobSize(minBlobSize)
{
}


CenterPointExtractor::~CenterPointExtractor()
{
}

void CenterPointExtractor::extract(const std::vector<LineSegment>& segments, std::vector<std::pair<float, float>>& centerPoints)
{
	short previousId = 0;

	int x = 0;
	int y = 0;
	int count = 0;
	centerPoints.clear();

	for (auto& line : segments)
	{
		if (previousId != line.blobId)
		{
			if (count > m_minBlobSize)
			{
				centerPoints.push_back(std::pair<float, float>(x / (float)count, y / (float)count));
			}
			int pixels = line.xRight - line.xLeft + 1;
			x = (pixels / 2 + line.xLeft) * pixels;
			y = line.y * pixels;
			count = pixels;
			previousId = line.blobId;
		}
		else
		{
			int pixels = line.xRight - line.xLeft + 1;
			x += (pixels / 2 + line.xLeft) * pixels;
			y += line.y * pixels;
			count += pixels;
		}
	}
	if (count > m_minBlobSize)
	{
		centerPoints.push_back(std::pair<float, float>(x / (float)count, y / (float)count));
	}
}