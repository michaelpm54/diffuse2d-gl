#include "window_sdl.h"

#include <stdexcept>
#include <cassert>

Window_SDL::Window_SDL(const WindowSettings &s)
{
	Create(s);
}

Window_SDL::~Window_SDL()
{
	if (!_destroyed)
	{
		Destroy();
	}
}

void Window_SDL::Create(const WindowSettings &s)
{
	bool doneInit = SDL_WasInit(SDL_INIT_VIDEO);
	if (!doneInit)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error("Failed to initialise SDL.");
		}
	}
	if (_handle)
	{
		SDL_DestroyWindow(_handle);
	}
	Uint32 flags = SDL_WINDOW_OPENGL | (s.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	_handle = SDL_CreateWindow(s.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, s.w, s.h, flags);
	if (!_handle)
	{
		throw std::runtime_error("Failed to create SDL window.");
	}
}

void Window_SDL::CreateGLContext(int glVerMajor, int glVerMinor, bool compat, bool doubleBuffer)
{
	assert(SDL_WasInit(SDL_INIT_VIDEO));
	assert(_handle);

	if (_ctx)
	{
		SDL_GL_DeleteContext(_ctx);
	}

	SDL_GLprofile profile = compat ? SDL_GL_CONTEXT_PROFILE_COMPATIBILITY : SDL_GL_CONTEXT_PROFILE_CORE;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glVerMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glVerMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffer);

	_ctx = SDL_GL_CreateContext(_handle);

	if (!_handle || !_ctx)
	{
		throw std::runtime_error("Failed to create GL context (version " + std::to_string(glVerMajor) + "." + std::to_string(glVerMinor) + ")");
	}
}

void Window_SDL::Destroy()
{
	if (_ctx)
	{
		SDL_GL_DeleteContext(_ctx);
	}
	SDL_DestroyWindow(_handle);
	SDL_Quit();
	_destroyed = true;
}

void Window_SDL::Update()
{
	SDL_GL_SwapWindow(_handle);
}

SDL_Window *Window_SDL::getHandle()
{
	return _handle;
}

SDL_GLContext Window_SDL::getGLContext()
{
	return _ctx;
}
