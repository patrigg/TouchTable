#pragma once
#include "../TouchDetector/PointTracker.h"

struct TouchEvent
{
	enum Type
	{
		Touch = 0,
		Move = 1,
		Release = 2
	} type;

	TrackedPoint point;
};

inline std::ostream& operator<<(std::ostream& s, TouchEvent::Type t)
{
	switch (t)
	{
	case 0:
		s << "Touch";
		break;
	case 1:
		s << "Move";
		break;
	case 2:
		s << "Release";
		break;
	}
	return s;
}