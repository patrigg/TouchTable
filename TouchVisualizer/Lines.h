#pragma once
#include <vector>

namespace sdl
{
	class Renderer;
}

class Lines
{
public:
	std::vector<std::pair<float, float>> points;

	void draw(sdl::Renderer& renderer);
};

