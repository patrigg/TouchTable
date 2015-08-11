#pragma once

namespace sdl
{
	class Renderer;
}

struct Cross
{
	float m_x;
	float m_y;
	void draw(sdl::Renderer& renderer);
};

