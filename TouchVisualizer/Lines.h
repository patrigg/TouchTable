#pragma once
#include <vector>

namespace sdl
{
	class Renderer;
}

class Lines
{
public:
	std::vector<std::pair<short, short>> points;

	void draw(sdl::Renderer& renderer);
};

