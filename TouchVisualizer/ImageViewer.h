#pragma once
#include <vector>
#include <SDLpp/Window.h>
#include <SDLpp/Renderer.h>
#include <SDLpp/Texture.h>
#include "Cross.h"
#include "Lines.h"
//#include "../SDLpp/GLContext.h"

#include "Picture.h"

class ImageViewer
{
	sdl::Window window;
	sdl::Renderer renderer;
	std::vector<Picture> pictures;
	
	//sdl::GLContext context;

public:
	std::vector<Cross> crosses;
	std::vector<Lines> lines;

	ImageViewer();

	void add(int x, int y, int width, int height);

	Picture& operator[](int i)
	{
		return pictures[i];
	}

	void update();
};

