#include "Configurator.h"
#include "IReceiver.h"
#include <sstream>


Configurator::Configurator(TouchTracking& tracking, EventSerializer& serializer)
	: tracking(tracking), serializer(serializer)
{
}


Configurator::~Configurator()
{
}

void Configurator::receive(const std::string& data, ISender& reply)
{
	
}

void Configurator::replyOk(int messageId, ISender& reply)
{
	std::stringstream s;
	s << "{\r\n\t\"message_id\": " << messageId << ",\r\n";
	s << "\t\"status\": \"ok\"\r\n";
	s << "}\r\n";
	reply.send(s.str());
}

void Configurator::replyFailed(int messageId, ISender& reply)
{
	std::stringstream s;
	s << "{\r\n\t\"message_id\": " << messageId << ",\r\n";
	s << "\t\"status\": \"failed\"\r\n";
	s << "}\r\n";
	reply.send(s.str());
}