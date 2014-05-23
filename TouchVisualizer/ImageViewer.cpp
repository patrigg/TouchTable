#include "stdafx.h"
#include "ImageViewer.h"
#include "Picture.h"
//#include "gl_core_4_3.hpp"


ImageViewer::ImageViewer()
: window("foo", 10, 10, 640, 480, sdl::Window::OpenGL), renderer(window)//, context(window)
{
	//context.makeCurrent(window);

	//gl::sys::LoadFunctions();
}

void ImageViewer::update()
{
	for (auto& picture : pictures)
	{
		picture.draw(renderer);
	}
	for (auto& cross : crosses)
	{
		cross.draw(renderer);
	}
	renderer.present();
	window.swap();

	//gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	//window.swap();
}

void ImageViewer::add(int x, int y, int width, int height)
{
	pictures.emplace_back(renderer, x, y, width, height);
}