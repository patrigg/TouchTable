#include "TestScanLineSegmenter.h"
#include <pbl/assert.h>
#include "../TouchDetector/Image.h"
#include "../TouchDetector/ScanLineSegmenter.h"
#include <algorithm>


TestScanLineSegmenter::TestScanLineSegmenter()
{
}


TestScanLineSegmenter::~TestScanLineSegmenter()
{
}

void TestScanLineSegmenter::run()
{
	runSingleLine();
	runMultipleLines();
	runMergeLines();
	runMultiMergeLines();
	runShapes();
	runMultiShapes();
}

void TestScanLineSegmenter::runSingleLine()
{
	Image<uint8_t> test(4, 1);
	
	ScanLineSegmenter segmenter;
	std::vector<LineSegment> segments;

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 0);

	test.data()[1] = 255;

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 1);

	PBL_ASSERT(segments[0].y == 0);
	PBL_ASSERT(segments[0].xLeft == 1);
	PBL_ASSERT(segments[0].xRight == 1);
	PBL_ASSERT(segments[0].blobId == 0);

	test.data()[2] = 255;

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 1);

	PBL_ASSERT(segments[0].y == 0);
	PBL_ASSERT(segments[0].xLeft == 1);
	PBL_ASSERT(segments[0].xRight == 2);
	PBL_ASSERT(segments[0].blobId == 0);

	test.data()[0] = 0;
	test.data()[1] = 255;
	test.data()[2] = 0;
	test.data()[3] = 255;

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 2);

	PBL_ASSERT(segments[0].y == 0);
	PBL_ASSERT(segments[0].xLeft == 1);
	PBL_ASSERT(segments[0].xRight == 1);
	PBL_ASSERT(segments[0].blobId == 0);

	PBL_ASSERT(segments[1].y == 0);
	PBL_ASSERT(segments[1].xLeft == 3);
	PBL_ASSERT(segments[1].xRight == 3);
	PBL_ASSERT(segments[1].blobId == 1);
	
}

void TestScanLineSegmenter::runMultipleLines()
{
	Image<uint8_t> test(4, 4);

	ScanLineSegmenter segmenter;
	std::vector<LineSegment> segments;

	test.data()[2] = 255;
	test.data()[3] = 255;

	test.data()[2 + 2 * 4] = 255;
	test.data()[3 + 2 * 4] = 255;

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 2);

	PBL_ASSERT(segments[0].y == 0);
	PBL_ASSERT(segments[0].xLeft == 2);
	PBL_ASSERT(segments[0].xRight == 3);
	PBL_ASSERT(segments[0].blobId == 0);

	PBL_ASSERT(segments[1].y == 2);
	PBL_ASSERT(segments[1].xLeft == 2);
	PBL_ASSERT(segments[1].xRight == 3);
	PBL_ASSERT(segments[1].blobId == 1);

	test.data() = std::vector<uint8_t> {
		1, 0, 0, 0,
		0, 1, 0, 1,
		1, 0, 1, 0,
		0, 1, 0, 1
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId != -1);
	}
}

void TestScanLineSegmenter::runMergeLines()
{
	Image<uint8_t> test(4, 4);

	ScanLineSegmenter segmenter;
	std::vector<LineSegment> segments;

	test.data()[2] = 255;
	test.data()[3] = 255;

	test.data()[2 + 1 * 4] = 255;
	test.data()[3 + 1 * 4] = 255;

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 2);

	PBL_ASSERT(segments[0].y == 0);
	PBL_ASSERT(segments[0].xLeft == 2);
	PBL_ASSERT(segments[0].xRight == 3);
	PBL_ASSERT(segments[0].blobId == 0);

	PBL_ASSERT(segments[1].y == 1);
	PBL_ASSERT(segments[1].xLeft == 2);
	PBL_ASSERT(segments[1].xRight == 3);
	PBL_ASSERT(segments[1].blobId == 0);

	test.data() = std::vector<uint8_t>{
		0, 255, 0, 255,
		0, 255, 255, 255,
		255, 0, 255, 0,
		255, 255, 255, 0
	};

	segmenter.segment(test, segments);
	PBL_ASSERT(segments.size() == 6);

	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}

}

void TestScanLineSegmenter::runMultiMergeLines()
{
	Image<uint8_t> test(15, 4);

	ScanLineSegmenter segmenter;
	std::vector<LineSegment> segments;

	test.data() = std::vector<uint8_t> {
		255, 0,   255, 0,   255, 0,   255, 0,   255, 0,   255, 0,   255, 0,   255,
		255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255, 0,   255, 255, 255,
		0,   0,   255, 255, 255, 0,   255, 0,   255, 0,   255, 255, 255, 0,   0,
		0,   0,   0,   0,   0,   0,   255, 255, 255, 0,   0,   0,   0,   0,   0
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}
}

void TestScanLineSegmenter::runShapes()
{
	Image<uint8_t> test(8, 8);

	ScanLineSegmenter segmenter;
	std::vector<LineSegment> segments;

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 0,
		0, 1, 1, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1, 1, 0,
		0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId == 0);
	}
}

void TestScanLineSegmenter::runMultiShapes()
{
	Image<uint8_t> test(10, 8);

	ScanLineSegmenter segmenter;
	std::vector<LineSegment> segments;

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 0, 0, 0, 1, 0,
		0, 1, 1, 1, 0, 0, 0, 0, 1, 0,
		0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId < 2);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 0, 1, 0,
		0, 1, 1, 1, 1, 0, 0, 0, 1, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId < 2);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 0, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId < 2);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId < 2);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId < 2);
	}

	test.data() = std::vector<uint8_t> {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	segmenter.segment(test, segments);
	for (auto& segment : segments)
	{
		PBL_ASSERT(segment.blobId < 2);
	}
}