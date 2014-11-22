#include "stdafx.h"
#include "Lines.h"
#include <SDLpp/Renderer.h>
#include <pbl/algorithm.h>

void Lines::draw(sdl::Renderer& renderer)
{
	renderer.drawColor(255, 0, 0, 255);

	pbl::adjacent_for_each(begin(points), end(points), [&renderer](const std::pair<double_t, double_t>& previous, const std::pair<double_t, double_t>& current)
	{
		renderer.drawLine(previous.first, previous.second, current.first, current.second);
	});
}