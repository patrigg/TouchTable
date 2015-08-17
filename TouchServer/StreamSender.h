#pragma once
#include "IReceiver.h"
#include <mutex>

class StreamSender : public ISender
{
	std::ostream& stream;
	std::mutex m;
public:
	StreamSender(std::ostream& stream);
	~StreamSender();

	void send(const std::string& data) override;
};

