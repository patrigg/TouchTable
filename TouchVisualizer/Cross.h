#pragma once

namespace sdl
{
	class Renderer;
}

struct Cross
{
	int m_x;
	int m_y;
	void draw(sdl::Renderer& renderer);
};

