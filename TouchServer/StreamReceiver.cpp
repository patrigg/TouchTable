#include "StreamReceiver.h"

StreamReceiver::StreamReceiver(std::istream& input, ISender& sender)
	: input(input), sender(sender)
{
}

StreamReceiver::~StreamReceiver()
{
}

void StreamReceiver::run()
{
	while (input.good())
	{
		std::string line;
		std::getline(input, line);
		onReceive(line, sender);
	}
}