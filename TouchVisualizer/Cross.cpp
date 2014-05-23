#include "stdafx.h"
#include "Cross.h"
#include "../SDLpp/Renderer.h"

void Cross::draw(sdl::Renderer& renderer)
{
	const int size = 3;
	renderer.drawColor(0, 0, 255, 255);
	renderer.drawLine(m_x, m_y - size, m_x, m_y + size);
	renderer.drawLine(m_x - size, m_y, m_x + size, m_y);
}
