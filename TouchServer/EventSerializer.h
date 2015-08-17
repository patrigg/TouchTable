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
	void mode(EventSerializer::Mode mode);

	EventSerializer(Mode mode);
	~EventSerializer();
private:
	Mode m_mode;
	void serializeBinary(std::ostream& stream, const TouchEvent& evt);
	void serializeJson(std::ostream& stream, const TouchEvent& evt);
};

