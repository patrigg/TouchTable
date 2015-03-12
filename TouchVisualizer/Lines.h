#pragma once
#include <vector>

namespace sdl
{
	class Renderer;
}

class Lines
{
public:
	std::vector<std::pair<double_t, double_t>> points;

	void draw(sdl::Renderer& renderer);
};

