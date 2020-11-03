#include "gl_util.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "renderer.h"
#include "gfx_device.h"

std::string readFile(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.good())
	{
		throw std::runtime_error("Unable to open file: " + filename);
	}

	std::stringstream ss;
	ss << file.rdbuf();

	return ss.str();
}

GLuint LoadAndCompileShader(const std::string &filename, GLenum shader_type)
{
	std::string source   = readFile(filename);
	auto        source_c = source.c_str();

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source_c, nullptr);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLsizei length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetShaderInfoLog(shader, length, nullptr, log.data());
		std::string log_str(log.begin(), log.end());
		std::cout << log_str << std::endl;
	}

	return shader;
}

void CreateShader(GLProgram &p, const std::string &vs_path, const std::string &fs_path)
{
	GLuint vs = LoadAndCompileShader(vs_path, GL_VERTEX_SHADER);
	GLuint fs = LoadAndCompileShader(fs_path, GL_FRAGMENT_SHADER);
	glAttachShader(p, vs);
	glAttachShader(p, fs);
	glLinkProgram(p);
	GLint status;
	glGetProgramiv(p, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLsizei length;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> log(length);
		glGetProgramInfoLog(p, length, nullptr, log.data());
		std::string log_str(log.begin(), log.end());
		std::cerr << log_str << std::endl;
	}
	glDetachShader(p, vs);
	glDetachShader(p, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

bool valueExistsInArray(int value, const std::vector<int> &array)
{
	return std::find(std::begin(array), std::end(array), value) != std::end(array);
}

static const std::vector<int> ValidInternalFormats =
{
	GL_DEPTH_COMPONENT,
	GL_DEPTH_STENCIL,
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA,

	GL_R8,
	GL_R8_SNORM,
	GL_R16,
	GL_R16_SNORM,
	GL_RG8,
	GL_RG8_SNORM,
	GL_RG16,
	GL_RG16_SNORM,
	GL_R3_G3_B2,
	GL_RGB4,
	GL_RGB5,
	GL_RGB8,
	GL_RGB8_SNORM,
	GL_RGB10,
	GL_RGB12,
	GL_RGB16_SNORM,
	GL_RGBA2,
	GL_RGBA4,
	GL_RGB5_A1,
	GL_RGBA8,
	GL_RGBA8_SNORM,
	GL_RGB10_A2,
	GL_RGB10_A2UI,
	GL_RGBA12,
	GL_RGBA16,
	GL_SRGB8,
	GL_SRGB8_ALPHA8,
	GL_R16F,
	GL_RG16F,
	GL_RGB16F,
	GL_RGBA16F,
	GL_R32F,
	GL_RG32F,
	GL_RGB32F,
	GL_RGBA32F,
	GL_R11F_G11F_B10F,
	GL_RGB9_E5,
	GL_R8I,
	GL_R8UI,
	GL_R16I,
	GL_R16UI,
	GL_R32I,
	GL_R32UI,
	GL_RG8I,
	GL_RG8UI,
	GL_RG16I,
	GL_RG16UI,
	GL_RG32I,
	GL_RG32UI,
	GL_RGB8I,
	GL_RGB8UI,
	GL_RGB16I,
	GL_RGB16UI,
	GL_RGB32I,
	GL_RGB32UI,
	GL_RGBA8I,
	GL_RGBA8UI,
	GL_RGBA16I,
	GL_RGBA16UI,
	GL_RGBA32I,
	GL_RGBA32UI,

	GL_COMPRESSED_RED,
	GL_COMPRESSED_RG,
	GL_COMPRESSED_RGB,
	GL_COMPRESSED_RGBA,
	GL_COMPRESSED_SRGB,
	GL_COMPRESSED_SRGB_ALPHA,
	GL_COMPRESSED_RED_RGTC1,
	GL_COMPRESSED_SIGNED_RED_RGTC1,
	GL_COMPRESSED_RG_RGTC2,
	GL_COMPRESSED_SIGNED_RG_RGTC2,
	GL_COMPRESSED_RGBA_BPTC_UNORM,
	GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
	GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
	GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
};

static const std::vector<int> ValidFormats =
{
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_BGR,
	GL_RGBA,
	GL_BGRA,
	GL_RED_INTEGER,
	GL_RG_INTEGER,
	GL_RGB_INTEGER,
	GL_BGR_INTEGER,
	GL_RGBA_INTEGER,
	GL_BGRA_INTEGER,
	GL_STENCIL_INDEX,
	GL_DEPTH_COMPONENT,
	GL_DEPTH_STENCIL
};

void UploadTexture(GLint internalFormat, GLsizei w, GLsizei h, GLenum format, GLenum type, const GLvoid *data)
{
	assert(valueExistsInArray(internalFormat, ValidInternalFormats));
	assert(valueExistsInArray(format, ValidFormats));

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, data);
}

/* Expects the texture to already be initialised by this point. */
void LoadTexture(GLTexture &t, const std::string &filename, bool flip, int *w_out, int *h_out)
{
	glBindTexture(GL_TEXTURE_2D, t);

	int c;
	int w;
	int h;

	stbi_set_flip_vertically_on_load(flip);
	stbi_uc *data = stbi_load(filename.c_str(), &w, &h, &c, STBI_rgb_alpha);
	if (!data)
	{
		std::cerr << "stbi error: " << stbi_failure_reason() << std::endl;
		throw std::runtime_error("Failed to load texture: " + filename);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	UploadTexture(GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	if (w_out)
	{
		*w_out = w;
	}

	if (h_out)
	{
		*h_out = h;
	}
}
