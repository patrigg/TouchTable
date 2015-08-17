#pragma once
#include <iostream>
#include <functional>
#include "IReceiver.h"

class StreamReceiver : public IReceiver
{
	std::istream& input;
	ISender& sender;
public:
	StreamReceiver(std::istream& input, ISender& sender);
	~StreamReceiver();

	void run() override;
};

