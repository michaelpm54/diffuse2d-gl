#ifndef WINDOW_H
#define WINDOW_H

struct WindowSettings
{
	const char *title;
	int        w;
	int        h;
	bool       fullscreen;
};

class Window
{
public:
	Window&operator =(const Window &rhs) = delete;
	Window(const Window &rhs) = delete;
	Window() {};
	virtual ~Window() {};
	virtual void Create(const WindowSettings &s) = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;
	virtual void CreateGLContext(int glVerMajor, int glVerMinor, bool compat, bool doubleBuffer) = 0;
};

#endif // WINDOW_H
