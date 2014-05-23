#include "stdafx.h"
#include "Picture.h"
#include "gl_core_4_3.hpp"
#include "../SDLpp/Renderer.h"
#include "../TouchDetector/Image.h"


namespace
{
	template<void(*create)(GLuint), void(*destroy)(GLuint)>
	class GLHandle
	{
		GLHandle()
		{
			create
		}
	};

	class Texture
	{
		GLuint tex;
	public: 
		Texture(Texture&) = delete;

		Texture()
		{
			gl::GenTextures(1, &tex);
		}

		~Texture()
		{
			gl::DeleteTextures(1, &tex);
		}

		GLuint get()
		{
			return tex;
		}
	};
}

Picture::Picture(sdl::Renderer& renderer, int x, int y, int width, int height)
: m_texture(renderer, sdl::Texture::RGB8, sdl::Texture::Streaming, width, height), m_x(x), m_y(y)
{

}

Picture::Picture(Picture&& rhs)
: m_texture(std::move(rhs.m_texture)), m_x(std::move(rhs.m_x)), m_y(std::move(rhs.m_y))
{

}

Picture& Picture::operator=(Picture&& rhs)
{
	m_texture = std::move(rhs.m_texture);
	m_x = std::move(rhs.m_x);
	m_y = std::move(rhs.m_y);
	return *this;
}

void Picture::draw(sdl::Renderer& renderer)
{
	renderer.copy(m_texture, m_x, m_y, m_texture.width(), m_texture.height());
}

void Picture::position(int x, int y)
{
	m_x = x;
	m_y = y;
}

template<>
void Picture::update(const Image<Color>& image)
{
	auto pixels = m_texture.lock();
	auto ptr = image.data().begin();
	for (auto row : pixels.rows<ColorAlpha>())
	{
		std::transform(ptr, ptr + image.width(), row.begin(), [](Color c){
			return ColorAlpha{ c.r, c.g, c.b, 256 };
		});
		ptr += image.width();
	}
}

template<>
void Picture::update(const Image<uint16_t>& image)
{
	auto pixels = m_texture.lock();
	auto ptr = image.data().begin();
	for (auto row : pixels.rows<ColorAlpha>())
	{
		std::transform(ptr, ptr + image.width(), row.begin(), [](uint16_t depth) {
			return ColorAlpha{ depth / 256, depth / 256, depth / 256, 256 };
		});
		ptr += image.width();
	}
}

template<>
void Picture::update(const Image<int>& image)
{
	auto pixels = m_texture.lock();
	auto ptr = image.data().begin();
	for (auto row : pixels.rows<ColorAlpha>())
	{
		std::transform(ptr, ptr + image.width(), row.begin(), [](int difference) {
			
			return ColorAlpha{ difference / 256 + 128, difference / 256 + 128, difference / 256 + 128, 256 };
		});
		ptr += image.width();
	}
}

template<>
void Picture::update(const Image<uint8_t>& image)
{
	auto pixels = m_texture.lock();
	auto ptr = image.data().begin();
	for (auto row : pixels.rows<ColorAlpha>())
	{
		std::transform(ptr, ptr + image.width(), row.begin(), [](uint8_t value) {

			return ColorAlpha{ value, value, value, 256 };
		});
		ptr += image.width();
	}
}