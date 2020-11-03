#include "gfx_device.h"

#include <stdexcept>

GfxDevice::GfxDevice()
{
	Create();
}

GfxDevice::~GfxDevice()
{
	if (!_destroyed)
	{
		Destroy();
	}
}

void GfxDevice::Create()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glweInit failed");

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void GfxDevice::Destroy()
{
	for (auto v : _vaos)
	{
		glDeleteVertexArrays(1, &v);
	}
	_vaos.clear();

	for (auto &p : _programs)
	{
		glDeleteProgram(p);
	}
	_programs.clear();

	for (auto &t : _textures)
	{
		glDeleteTextures(1, &t);
	}
	_textures.clear();

	for (auto &b : _buffers)
	{
		glDeleteBuffers(1, &b);
	}
	_buffers.clear();

	_destroyed = true;
}

GLVao GfxDevice::NewVao()
{
	GLVao &v = _vaos.emplace_back();
	glCreateVertexArrays(1, &v);
	return v;
}

GLProgram GfxDevice::NewProgram()
{
	GLProgram &p = _programs.emplace_back();
	p = glCreateProgram();
	return p;
}

GLBuffer GfxDevice::NewBuffer()
{
	GLBuffer &b = _buffers.emplace_back();
	glCreateBuffers(1, &b);
	return b;
}

GLTexture GfxDevice::NewTexture()
{
	GLTexture &t = _textures.emplace_back();
	glCreateTextures(GL_TEXTURE_2D, 1, &t);
	return t;
}
