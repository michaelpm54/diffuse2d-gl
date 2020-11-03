#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <map>
#include <memory>
#include "gfx_device.h"
#include "gl_util.h"

struct Font
{
	int charSizeX;
	int charSizeY;
	GLuint texture;
};

class FontManager
{
public:
	static std::shared_ptr<Font> LoadBitmapFont(GfxDevice &d, const std::string &path, int x, int y)
	{
		if (_fonts.count(path))
		{
			return _fonts[path];
		}
		else
		{
			GLuint texture = d.NewTexture();
			try {
				int w, h;
				LoadTexture(texture, path, false, &w, &h);
				auto font = std::make_shared<Font>();
				font->charSizeX = w / x;
				font->charSizeY = h / y;
				font->texture = texture;
				_fonts.insert({path, font});
				return font;
			} catch (const std::exception &e) {
				throw std::runtime_error("Failed loading font '" + path + "' with error '" + e.what() + "'");
			}
		}
	}

	static FontManager &instance()
	{
		if (!s)
		{
			s = FontManager::makeInstance();
		}
		return *s;
	}

	static void destroy()
	{
		FontManager::destroyInstance();
	}

private:
	static FontManager *makeInstance()
	{
		return new FontManager();
	}

	static void destroyInstance()
	{
		delete s;
		s = nullptr;
	}

	static FontManager *s;

	static std::map<const std::string, std::shared_ptr<Font>> _fonts;
};

FontManager *FontManager::s = 0;
std::map<const std::string, std::shared_ptr<Font>> FontManager::_fonts;

#define FontMan (FontManager::instance())

void DrawText(Font &font, float x, float y, const std::string &text);

#endif // FONT_MANAGER_H
