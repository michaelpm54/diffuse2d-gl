#include "engine.h"

#include <iostream>

#include <SDL2/SDL.h>

#include "gfx_device.h"
#include "renderer.h"
#include "window_sdl.h"
#include "timer.h"
#include "world.h"

Engine::Engine() : _timer() {};

Engine::~Engine()
{
	/* Make sure the GL stuff is released before the context is deleted. */
	_gfx->Destroy();
	_win->Destroy();
}

void Engine::Start()
{
	Init();
	Loop();
}

void Engine::Init()
{
	_timer.Reset();
	_win = std::make_unique<Window_SDL>(WindowSettings{"Engine", 640, 640, false});
	_win->CreateGLContext(4, 6, false, false);
	_ren = std::make_shared<Renderer>();
	_ren->SetClearColour(0, 0.5, 0, 1);
	_gfx = std::make_shared<GfxDevice>();
	_world = std::make_shared<World>(_gfx, _ren);

	std::cout << "Engine init took " << _timer.msElapsedSinceReset() << " ms" << std::endl;
}

void Engine::Loop()
{
	_timer.Reset();
	int simulationTime = 0;

	_world->Load("assets/worlds/intro.wd");

	while (!_quitRequested)
	{
		int realTime = _timer.msElapsedSinceReset();

		_world->Input();

		while (simulationTime < realTime)
		{
			simulationTime += 16;
			_world->Update(16);
		}

		_quitRequested = _world->quitRequested();

		_ren->Clear();
		_world->Render();
		_win->Update();
		SDL_Delay(16);
	}
}
