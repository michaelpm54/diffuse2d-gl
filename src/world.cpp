#include "world.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfx_device.h"
#include "gl_util.h"
#include "renderer.h"
#include "game_object.h"
#include "render_component.h"
#include "font_manager.h"

#include <SDL2/SDL.h>

#define SCALE 0.2f

std::vector<float> GenQuadVertices(float x = 0, float y = 0, float z = 0)
{
	std::vector<float> v(6 * 8);

	v =
	{
		/*pos*/ 0+x, 0+y, 0+z, /*uv*/ 0, 0, /*norm*/ 0+x, 0+y, -1+z,
		/*pos*/ 1+x, 0+y, 0+z, /*uv*/ 1, 0, /*norm*/ 0+x, 0+y, -1+z,
		/*pos*/ 1+x, 1+y, 0+z, /*uv*/ 1, 1, /*norm*/ 0+x, 0+y, -1+z,
		/*pos*/ 0+x, 0+y, 0+z, /*uv*/ 0, 0, /*norm*/ 0+x, 0+y, -1+z,
		/*pos*/ 1+x, 1+y, 0+z, /*uv*/ 1, 1, /*norm*/ 0+x, 0+y, -1+z,
		/*pos*/ 0+x, 1+y, 0+z, /*uv*/ 0, 1, /*norm*/ 0+x, 0+y, -1+z,
	};

	return v;
}

World::World() = default;

World::World(std::shared_ptr<GfxDevice> gfx, std::shared_ptr<Renderer> ren)
	:
	_gfx(gfx),
	_ren(ren)
{
	_camera = glm::ortho(0.0f, 640.0f * SCALE, 640.0f * SCALE, 0.0f);
	// _camera = glm::perspective(glm::radians(45.0f), (float)(640.0f * SCALE)/(float)(640.0f * SCALE), 0.1f, 400.0f);
	// pos, origin, eye
	// glm::vec3 pos(64, 64, -175);
	// glm::vec3 ori(pos.x, pos.y, -pos.z);
	// _camera *= glm::lookAt(pos, ori, glm::vec3(0, -1, 0));
}

World::~World()
{
}

std::shared_ptr<GameObject> World::AddGameObject()
{
	std::shared_ptr<GameObject> obj = _objs.emplace_back(std::make_shared<GameObject>());
	return obj;
}

std::shared_ptr<GameObject> World::AddObject(glm::vec2 pos, glm::vec2 size, std::vector<float> &vertices, GLTexture tex, GLProgram program)
{
	std::shared_ptr<GameObject> obj(AddGameObject());

	obj->SetPosition(pos);
	obj->SetSize(size);
	obj->SetTexture(tex);
	obj->GetRenderComponent()->SetProgram(program);
	GLVao vao = _gfx->NewVao();
	GLBuffer vbo = _gfx->NewBuffer();
	_ren->BufferData(vbo, vertices);
	_ren->BindVao(vao);
	_ren->BindBuffer(vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	_ren->UnbindBuffer();
	_ren->UnbindVao();
	obj->GetRenderComponent()->SetVao(vao);

	return obj;
}

void World::Load(const std::string &worldPath)
{
	std::cout << "Loading world: \"" << worldPath << "\"" << std::endl;

	_timer.Reset();

	const std::string world = readFile(worldPath);
	if (world.empty())
	{
		std::cerr << "World was empty: \"" << worldPath << "\"" << std::endl;
		return;
	}

	_tm.SetSize(4, 4);

	int j         = 0;
	std::size_t i = 0;
	while (i < world.size())
	{
		int id = 0;
		sscanf(world.c_str() + i, "%x", &id);
		int x = j % 4;
		int y = j / 4;
		_tm.SetTile(x, y, id);
		i += 3;
		j++;
	}

	GLProgram quadProgram = _gfx->NewProgram();
	CreateShader(quadProgram, "assets/shaders/quad.v.glsl", "assets/shaders/quad.f.glsl");

	const int tileSizeX = 32;
	const int tileSizeY = 32;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			GLTexture tex;
			try {
				GetTileTexture(tex, _tm.getTile(x, y));
			} catch (const std::exception &e) {
				std::cerr << e.what() << std::endl;
				continue;
			}
			auto vertices = GenQuadVertices();
			AddObject({x*tileSizeX, y*tileSizeY}, {tileSizeX, tileSizeY}, vertices, tex, quadProgram);
		}
	}

	std::cout << "Loaded " << _tm.count() << " tiles in " << _timer.msElapsedSinceReset() << " ms" << std::endl;

	// _tm.print(std::cout);
	// std::cout << std::endl;

	{
		const int heroSizeX = 20;
		const int heroSizeY = 20;

		int a = tileSizeX - heroSizeX;
		int b = tileSizeY - heroSizeY;

		GLTexture heroTex;
		try {
			_ren->GetTexture(_gfx.get(), "assets/textures/hero.png", heroTex);
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		
		auto vertices = GenQuadVertices(0, 0, 0);
		_hero = AddObject({a/2, b/2}, {heroSizeX, heroSizeY}, vertices, heroTex, quadProgram);
	}

	/* Load fonts. */
	_bitmapFont = FontMan.LoadBitmapFont(*_gfx, "assets/fonts/Nice_curses_12x12.png", 16, 16);
}

/* xDir and yDir represent directions, not magnitude.
   -1 = negative directio
    1 = positive direction
    0 = not moving
 */
void World::MoveQuad(Quad &quad, int xDir, int yDir)
{
	// xDir *= (32.0f / _sim);
	// yDir *= (32.0f / _sim);
	quad.pos.x += (xDir * _interpolationAmt);
	quad.pos.y += (yDir * _interpolationAmt);
	quad.model  = glm::mat4(1.0f);
	quad.model  = glm::translate(quad.model, glm::vec3(quad.pos.x, quad.pos.y, 0));
	quad.model  = glm::scale(quad.model, glm::vec3(quad.size.x, quad.size.y, 1));
}

void World::QueueMove(int xDir, int yDir, int steps)
{
	if (_queueMove)
	{
		return;
	}
	_moveX            = xDir;
	_moveY            = yDir;
	_queueMove        = true;
	_moveSteps        = steps;
	_interpolationAmt = (32.0f / _moveSteps);
}

static constexpr int MoveSteps = 8;

void World::Input()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
			case SDL_QUIT:
				_quitRequested = true;
				break;
			case SDL_KEYDOWN:
				switch (evt.key.keysym.sym)
				{
					case SDLK_q:
						_quitRequested = true;
						break;
					case SDLK_w:
						QueueMove(0,  -1, MoveSteps);
						break;
					case SDLK_a:
						QueueMove(-1, 0, MoveSteps);
						break;
					case SDLK_s:
						QueueMove(0,  1, MoveSteps);
						break;
					case SDLK_d:
						QueueMove(1,  0, MoveSteps);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

void World::Update(int increment)
{
	_sim = increment;

	if (_queueMove && (_moveSteps != 0))
	{
		_hero->Move(_moveX * _interpolationAmt, _moveY * _interpolationAmt);
		--_moveSteps;
		if (_moveSteps == 0)
		{
			_queueMove = false;
		}
	}
}

void World::GetTileTexture(GLTexture &tex, int tileId)
{
	assert(tileId >= 0 && tileId <= 3);
	std::string filename = std::to_string(tileId);
	filename = std::string(2 - filename.length(), '0') + filename + ".png";
	_ren->GetTexture(_gfx.get(), "assets/textures/" + filename, tex);
}

void World::MakeQuad(int x, int y, int w, int h, GLTexture tex)
{
	Quad quad;
	quad.pos  = { x, y };
	quad.size = { w, h };
	quad.tex  = tex;
	quad.vao  = _gfx->NewVao();
	quad.prog = _gfx->NewProgram();
	CreateShader(quad.prog, "assets/shaders/quad.v.glsl", "assets/shaders/quad.f.glsl");

	GLBuffer vbo = _gfx->NewBuffer();
	_ren->BufferData(vbo, GenQuadVertices());
	quad.model = glm::mat4(1.0f);
	quad.model = glm::translate(quad.model, glm::vec3(x, y, 0));
	quad.model = glm::scale(quad.model, glm::vec3(w, h, 1));

	_ren->BindVao(quad.vao);
	_ren->BindBuffer(vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	_ren->UnbindBuffer();
	_ren->UnbindVao();

	_quads.push_back(std::move(quad));
}

void World::RenderQuad(const Quad &quad)
{
	_ren->BindVao(quad.vao);
	_ren->BindTexture(quad.tex);
	_ren->UniformMatrix("model",  glm::value_ptr(quad.model));
	_ren->DrawTris(0, 6);
}

void World::Render()
{
	for (const auto &obj : _objs)
	{
		// if (obj != _hero)
		{
			GLProgram p = obj->GetRenderComponent()->getProgram();
			_ren->BindProgram(p);
			glm::vec3 heroSizeHalf(glm::vec3(_hero->getSize(), 0.0f));
			heroSizeHalf.x /= 2;
			heroSizeHalf.y /= 2;
			glm::vec3 heroPos(glm::vec3(_hero->getPosition(), -20.0f) + heroSizeHalf);
			GLint light = glGetUniformLocation(p, "lightPos");
			glUniform3fv(light, 1, glm::value_ptr(heroPos));
		}
		_ren->Render(obj->GetRenderComponent(), glm::value_ptr(_camera), glm::value_ptr(obj->getTransform()));
	}
}

bool World::quitRequested() const
{
	return _quitRequested;
}
