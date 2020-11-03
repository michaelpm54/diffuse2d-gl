#ifndef GL_UTIL_H
#define GL_UTIL_H

#include <string>

#include "gl_types.h"
#include "renderer.h"
#include "gfx_device.h"

std::string readFile(const std::string &filename);
void CreateShader(GLProgram &p, const std::string &vs_path, const std::string &fs_path);

/* The texture should be initialised before this function is called. */
void LoadTexture(GLTexture &tex, const std::string &filename, bool flip, int *w = nullptr, int *h = nullptr);

#endif // GL_UTIL_H
