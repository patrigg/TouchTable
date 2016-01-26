#include "EventSerializer.h"
#include "TouchEvent.h"
#include <ctime>

EventSerializer::EventSerializer(Mode mode)
	: m_mode(mode)
{
}


EventSerializer::~EventSerializer()
{
}

void EventSerializer::serialize(std::ostream& stream, const TouchEvent& evt)
{
	switch (m_mode)
	{
	case Mode::Binary:
		return serializeBinary(stream, evt);
	case Mode::Json:
		return serializeJson(stream, evt);
	}
}

void EventSerializer::serializeBinary(std::ostream& stream, const TouchEvent& evt)
{
	const char b = 1;
	char type = evt.type + 1;
	stream.write(&b, sizeof(char));
	stream.write(reinterpret_cast<const char*>(&evt.point.currentTimestamp), sizeof(int));
	stream.write(reinterpret_cast<const char*>(&evt.point.id), sizeof(int));
	stream.write(reinterpret_cast<const char*>(&type), sizeof(char));
	stream.write(reinterpret_cast<const char*>(&evt.point.position.first), sizeof(float));
	stream.write(reinterpret_cast<const char*>(&evt.point.position.second), sizeof(float));
}

namespace {
	void print_time(std::ostream& stream, std::chrono::system_clock::time_point time)
	{
		auto micros_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
		auto micros = micros_since_epoch % 1000;
		auto whole_seconds = std::chrono::system_clock::time_point(time.time_since_epoch() - std::chrono::microseconds(micros));
	}
}

void EventSerializer::serializeJson(std::ostream& stream, const TouchEvent& evt)
{
	stream << "{\r\n\t\"type\": \"" << evt.type << "\",\r\n";
	//stream << "\t\"frame_time\": \"";
	//print_time(stream, evt.frameTime);
	//stream << "\",\r\n";
	stream << "\t\"frame_id\": " << evt.point.currentTimestamp << ",\r\n";
	stream << "\t\"id\": " << evt.point.id << ",\r\n";
	stream << "\t\"position\": {\r\n";
	stream << "\t\t\"x\": " << evt.point.position.first << ",\r\n";
	stream << "\t\t\"y\": " << evt.point.position.second << ",\r\n";
	stream << "\t}\r\n";
	stream << "}\r\n";
}

void EventSerializer::mode(EventSerializer::Mode mode)
{
	m_mode = mode;
}

EventSerializer::Mode EventSerializer::mode() const
{
	return m_mode;
}
