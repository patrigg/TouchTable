#pragma once
class TestScanLineSegmenter
{
public:
	TestScanLineSegmenter();
	~TestScanLineSegmenter();

	void run();

	void runSingleLine();
	void runMultipleLines();
	void runMergeLines();
	void runMultiMergeLines();
	void runShapes();
	void runMultiShapes();
};

