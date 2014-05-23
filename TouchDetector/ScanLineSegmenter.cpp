#include "ScanLineSegmenter.h"
#include "Image.h"
#include <algorithm>

namespace
{
	void advanceLeftmost(std::vector<LineSegment>::iterator& first, std::vector<LineSegment>::iterator& second)
	{
		if (first->xRight <= second->xRight)
		{
			++first;
		}
		else
		{
			++second;
		}
	}

	struct CreateMapping
	{
		std::vector<short>& mapping;

		void operator()(LineSegment& line)
		{
			line.blobId = mapping.size();
			mapping.push_back(line.blobId);
		}
	};

	template<typename T>
	void combineSegments(T previousFirst, T previousLast, T first, T last, std::vector<short>& mapping)
	{
		CreateMapping createMapping = { mapping };

		if (previousFirst == previousLast || first != last && (previousFirst->y != (first->y - 1)))
		{
			std::for_each(first, last, createMapping);
			return;
		}

		while (previousFirst != previousLast && first != last)
		{
			if (previousFirst->xRight < first->xLeft)
			{
				++previousFirst;
			}
			else if (first->xRight < previousFirst->xLeft)
			{
				if (first->blobId == -1)
				{
					createMapping(*first);
				}
				++first;
			}
			else
			{
				if (first->blobId != -1)
				{
					if (mapping[first->blobId] != mapping[previousFirst->blobId] && first->blobId != previousFirst->blobId)
					{
						short ids[4] = { first->blobId, previousFirst->blobId, mapping[first->blobId], mapping[previousFirst->blobId] };
						auto minimumId = *std::min_element(std::begin(ids), std::end(ids));
						for (auto id : ids)
						{
							mapping[id] = minimumId;
						}
					}
				}
				else
				{
					first->blobId = previousFirst->blobId;
				}

				advanceLeftmost(first, previousFirst);
			}
		}

		if (first != last && first->blobId != -1)
		{
			++first;
		}
		std::for_each(first, last, createMapping);
	}
}

bool byBlobId(const LineSegment& lhs, const LineSegment& rhs)
{
	return lhs.blobId < rhs.blobId;
}

void ScanLineSegmenter::segment(const Image<uint8_t>& image, std::vector<LineSegment> & segmented)
{
	std::vector<LineSegment>& lineSegments = segmented;
	lineSegments.clear();

	auto it = image.data().begin();

	for (int y = 0; y < image.height(); ++y)
	{
		LineSegment currentSegment{ y, 0, 0, -1 };

		short length = 0;

		for (int x = 0; x < image.width(); ++x)
		{
			if (*it)
			{
				++length;
			}
			else
			{
				if (length)
				{
					currentSegment.xRight = currentSegment.xLeft + length - 1;
					lineSegments.push_back(currentSegment);
					length = 0;
				}
				currentSegment.xLeft = x + 1;
			}
			++it;
		}
		if (length)
		{
			currentSegment.xRight = currentSegment.xLeft + length - 1;
			lineSegments.push_back(currentSegment);
		}
	}

	std::vector<short> mapping;
	{
		auto lineEnd = begin(lineSegments);
		auto previousLineBegin = begin(lineSegments);
		auto previousLineEnd = begin(lineSegments);
		const auto last = end(lineSegments);
		do {
			lineEnd = std::adjacent_find(previousLineEnd, last, [](const LineSegment& lhs, const LineSegment& rhs) {
				return lhs.y != rhs.y;
			});

			lineEnd += lineEnd != last;

			combineSegments(previousLineBegin, previousLineEnd, previousLineEnd, lineEnd, mapping);

			previousLineBegin = previousLineEnd;
			previousLineEnd = lineEnd;

		} while (lineEnd != last);

		for (auto& line : lineSegments)
		{
			while (mapping[line.blobId] != line.blobId)
			{
				line.blobId = mapping[line.blobId];
			}
		}
	}

	std::stable_sort(begin(lineSegments), end(lineSegments), byBlobId);
}