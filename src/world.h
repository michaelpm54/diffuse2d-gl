#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "app_gl.h"

#include <glm/glm.hpp>

#include "timer.h"
#include "tilemap.h"
#include "gl_types.h"

class GfxDevice;
class Renderer;
class GameObject;
class Font;

struct Mesh
{
	glm::mat4 model;
	GLTexture tex;
	GLProgram prog;
	GLVao vao;
};

struct vertex_t
{
	glm::vec2 pos;
	glm::vec2 uv;
};

struct Quad : public Mesh
{
	glm::vec2 pos;
	glm::vec2 size;
};

class World
{
public:
	World();
	World(std::shared_ptr<GfxDevice> gfx, std::shared_ptr<Renderer> ren);
	~World();

	void Input();
	void Load(const std::string &worldPath);
	void Update(int increment);
	void Render();

	void GetTileTexture(GLTexture &tex, int tileId);

	void MakeQuad(int x, int y, int w, int h, GLTexture tex);
	void RenderQuad(const Quad &quad);
	void MoveQuad(Quad &quad, int x, int y);
	void QueueMove(int xDir, int yDir, int steps);

	std::shared_ptr<GameObject> AddGameObject();
	std::shared_ptr<GameObject> AddObject(glm::vec2 pos, glm::vec2 size, std::vector<float> &vertices, GLTexture tex, GLProgram program);

	bool quitRequested() const;

private:
	std::shared_ptr<GfxDevice> _gfx;
	std::shared_ptr<Renderer> _ren;
	Timer _timer;
	TileMap _tm;
	std::vector<Quad> _quads;
	glm::mat4 _camera;
	bool _quitRequested { false };
	bool _queueMove { false };
	int _moveX { 0 };
	int _moveY { 0 };
	int _moveSteps { 0 };
	float _interpolationAmt { 0.0 };
	int _sim { 0 };
	std::shared_ptr<GameObject> _hero;
	std::vector<std::shared_ptr<GameObject>> _objs;
	std::shared_ptr<Font> _bitmapFont;
};

#endif // WORLD_H
