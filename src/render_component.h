#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "gl_types.h"

class RenderComponent
{
public:
	void SetTexture(GLTexture);
	GLTexture getTexture() const;

	void SetProgram(GLProgram);
	GLProgram getProgram() const;

	void SetVao(GLVao);
	GLVao getVao() const;

private:
	GLTexture _texture { 0 };
	GLProgram _program { 0 };
	GLVao _vao { 0 };
};

#endif // RENDER_COMPONENT_H
