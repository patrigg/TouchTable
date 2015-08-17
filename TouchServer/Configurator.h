#pragma once
#include <string>
#include "TouchTracking.h"
#include "EventSerializer.h"

class ISender;

class Configurator
{
	TouchTracking& tracking;
	EventSerializer& serializer;
public:
	Configurator(TouchTracking& tracking, EventSerializer& serializer);
	~Configurator();

	void receive(const std::string& data, ISender& reply);
	void replyOk(int messageId, ISender& reply);
	void replyFailed(int messageId, ISender& reply);
};

