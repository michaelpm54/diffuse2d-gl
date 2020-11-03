#include "renderer.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "gfx_device.h"
#include "gl_util.h"
#include "render_component.h"

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetClearColour(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::DrawTris(GLuint start, GLuint end)
{
	glDrawArrays(GL_TRIANGLES, start, end);
}

void Renderer::BufferData(GLBuffer b, const std::vector<GLfloat> &v)
{
	glBindBuffer(GL_ARRAY_BUFFER, b);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(GLfloat), v.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::BindVao(GLVao v)
{
	if (_vao == v)
	{
		return;
	}
	else
	{
		_vao = v;
	}
	glBindVertexArray(v);
}

void Renderer::BindBuffer(GLBuffer b)
{
	if (_arrayBuffer == b)
	{
		return;
	}
	else
	{
		_arrayBuffer = b;
	}
	glBindBuffer(GL_ARRAY_BUFFER, b);
}

void Renderer::BindProgram(GLProgram p)
{
	if (_program == p)
	{
		return;
	}
	else
	{
		_program = p;
	}
	glUseProgram(_program);
}

void Renderer::BindTexture(GLTexture t)
{
	if (t == _texture)
	{
		return;
	}
	else
	{
		_texture = t;
	}
	// std::cout << "Binding texture " << t << " (" << _texturesReverse[t] << ")" << std::endl;
	glBindTexture(GL_TEXTURE_2D, t);
}

void Renderer::SetTextureFilter(GLint param)
{
	SetTextureParam(GL_TEXTURE_MIN_FILTER, param);
	SetTextureParam(GL_TEXTURE_MAG_FILTER, param);
}

void Renderer::SetTextureWrap(GLint param)
{
	SetTextureParam(GL_TEXTURE_WRAP_S, param);
	SetTextureParam(GL_TEXTURE_WRAP_T, param);
}

void Renderer::SetTextureParam(GLenum pname, GLint param)
{
	glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Renderer::UnbindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::UnbindVao()
{
	glBindVertexArray(0);
}

void Renderer::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::UnbindProgram()
{
	BindProgram(0);
}

void Renderer::UniformMatrix(const std::string &name, const GLfloat * const mat)
{
	GLint loc = -1;
	if (!_uniforms.count(name))
	{
		std::cout << "Adding uniform '" << name << "' for program " << _program << " ... ";
		loc = glGetUniformLocation(_program, name.c_str());
		if (loc == -1)
		{
			std::cout << "Uniform doesn't exist" << std::endl;
			return;
		}
		else
		{
			std::cout << "Uniform found [" << loc << "]" << std::endl;
			_uniforms.insert(std::make_pair(name, loc));
		}
	}
	loc = _uniforms[name];
	glUniformMatrix4fv(loc, 1, GL_FALSE, mat);

	/*
	GLfloat mat4[16];
	glGetUniformfv(_program, loc, mat4);
	std::cout << "Program " << _program << " uniform " << loc << " '" << name << "' contents: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << mat4[i*4+j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
}

void Renderer::GetTexture(GfxDevice *gfx, const std::string &path, GLTexture &texture)
{
	if (!_textures.count(path))
	{
		std::cout << "Adding texture '" << path << "'... ";

		try {
			texture = gfx->NewTexture();
			LoadTexture(texture, path, false);
		} catch (const std::exception &e) {
			throw std::runtime_error("Texture couldn't be loaded");
		}

		std::cout << "Texture loaded with ID " << texture << std::endl;
		_textures.insert(std::make_pair(path, texture));
		_texturesReverse.insert(std::make_pair(texture, path));
	}
	else
	{
		texture = _textures[path];
	}
}

void Renderer::Render(const std::shared_ptr<RenderComponent> &c, const GLfloat * const cam, const GLfloat * const model)
{
	BindProgram(c->getProgram());
	BindVao(c->getVao());
	BindTexture(c->getTexture());
	UniformMatrix("camera", cam);
	UniformMatrix("model", model);
	DrawTris(0, 6);
	UnbindVao();
}
