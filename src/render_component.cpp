#include "render_component.h"

void RenderComponent::SetTexture(GLTexture tex)
{
	_texture = tex;
}

GLTexture RenderComponent::getTexture() const
{
	return _texture;
}

void RenderComponent::SetProgram(GLProgram program)
{
	_program = program;
}

GLProgram RenderComponent::getProgram() const
{
	return _program;
}

void RenderComponent::SetVao(GLVao vao)
{
	_vao = vao;
}

GLVao RenderComponent::getVao() const
{
	return _vao;
}
