#pragma once
#include <ostream>

struct TouchEvent;

class EventSerializer
{
public:
	enum class Mode
	{
		Binary,
		Json
	};

	void serialize(std::ostream& stream, const TouchEvent& evt);
	EventSerializer(Mode mode);
	~EventSerializer();
private:
	Mode mode;
	void serializeBinary(std::ostream& stream, const TouchEvent& evt);
	void serializeJson(std::ostream& stream, const TouchEvent& evt);
};

