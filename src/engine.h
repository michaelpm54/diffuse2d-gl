#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "timer.h"

class Window;
class GfxDevice;
class Renderer;
class World;

class Engine
{
public:
	Engine();
	~Engine();

	void Start();
private:
	void Init();
	void Loop();

	std::shared_ptr<Window> _win;
	std::shared_ptr<GfxDevice> _gfx;
	std::shared_ptr<Renderer> _ren;
	std::shared_ptr<World> _world;
	Timer _timer;
	bool _quitRequested { false };
};

#endif // ENGINE_H
