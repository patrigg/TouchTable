#pragma once
#include <string>
#include "TouchTracking.h"
#include "EventSerializer.h"
#include "json\json-forwards.h"

class ISender;

class Configurator
{
	TouchTracking& tracking;
	EventSerializer& serializer;
	void update(const std::string& name, const Json::Value& value);

public:
	Configurator(TouchTracking& tracking, EventSerializer& serializer);
	~Configurator();

	void receive(const std::string& data, ISender& reply);
	void replyOk(int messageId, ISender& reply);
	void replyFailed(int messageId, ISender& reply);
};

