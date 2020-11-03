#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <memory>
#include <vector>

#include "gl_types.h"

class GfxDevice;
class RenderComponent;

class Renderer
{
public:
	void SetClearColour(float r, float g, float b, float a);
	void Clear();
	void DrawTris(GLuint start, GLuint end);
	void BindProgram(GLProgram);
	void BindVao(GLVao);
	void BindTexture(GLTexture);
	void BindBuffer(GLBuffer);
	void BufferData(GLBuffer, const std::vector<GLfloat>&);
	void UnbindBuffer();
	void UnbindVao();
	void UnbindTexture();
	void UnbindProgram();

	void GetTexture(GfxDevice *gfx, const std::string &path, GLTexture &texture);

	void SetTextureFilter(GLint param);
	void SetTextureWrap(GLint param);
	void SetTextureParam(GLenum pname, GLint param);

	void UploadTexture(GLint internalFormat, GLsizei w, GLsizei h, GLenum format, GLenum type, const GLvoid *data);

	void UniformMatrix(const std::string &name, const GLfloat * const mat);

	void Render(const std::shared_ptr<RenderComponent>&, const GLfloat * const, const GLfloat * const);

private:
	void TextureParam(GLenum pname, GLint param);

	std::map<const std::string, const GLint> _uniforms;
	std::map<const std::string, const GLTexture> _textures;
	std::map<const GLTexture, const std::string> _texturesReverse;
	GLProgram _program { 0 };
	GLTexture _texture { 0 };
	GLBuffer _arrayBuffer { 0 };
	GLVao _vao { 0 };
};

#endif // RENDERER_H
