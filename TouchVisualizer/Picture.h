#pragma once
#include <SDLpp/Texture.h>

template<typename T>
class Image;

namespace sdl
{
	class Renderer;
}

class Picture
{
	sdl::Texture m_texture;
	int m_x;
	int m_y;
	
	Picture(const Picture&) = delete;
	Picture& operator=(const Picture&) = delete;

public:
	Picture(sdl::Renderer& renderer, int x, int y, int width, int height);
	Picture(Picture&& rhs);
	Picture& operator=(Picture&& rhs);
	void draw(sdl::Renderer& renderer);

	void position(int x, int y);

	template<typename T>
	void update(const Image<T>& image);
};

