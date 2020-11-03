#ifndef GFX_DEVICE_GL
#define GFX_DEVICE_GL

#include <vector>

#include "gl_types.h"

/*
    This is a class to facilitate aquisition and destruction
    of OpenGL resources such as textures, programs, and buffers.
    When this class is destroyed, all resources acquired through it
    are too.

    The GLHandles provided by this class are meant to be used by
    the Renderer class.
 */

class GfxDevice
{
public:
	GfxDevice();
	~GfxDevice();
	void      Create();
	void      Destroy();
	GLVao     NewVao();
	GLProgram NewProgram();
	GLBuffer  NewBuffer();
	GLTexture NewTexture();

private:
	bool _destroyed { false };
	std::vector<GLVao> _vaos;
	std::vector<GLProgram> _programs;
	std::vector<GLTexture> _textures;
	std::vector<GLBuffer> _buffers;
};

#endif // GFX_DEVICE_GL
