#include "StreamSender.h"


StreamSender::StreamSender(std::ostream& stream)
	: stream(stream)
{
}


StreamSender::~StreamSender()
{
}
void StreamSender::send(const std::string& data)
{
	std::lock_guard<std::mutex> l(m);
	stream << data;
}