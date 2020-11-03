#ifndef WINDOW_SDL_H
#define WINDOW_SDL_H

#define SDL_MAIN_HANDLED
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "window.h"

class Window_SDL : public Window {
public:
  Window_SDL() = default;
  Window_SDL(const WindowSettings &s);
  virtual ~Window_SDL() override;
  virtual void Create(const WindowSettings &s) override;
  virtual void Destroy() override;
  virtual void Update() override;
  virtual void CreateGLContext(int glVerMajor, int glVerMinor, bool compat,
                               bool doubleBuffer) override;

  SDL_Window *getHandle();
  SDL_GLContext getGLContext();

private:
  SDL_Window *_handle{nullptr};
  SDL_GLContext _ctx{nullptr};
  bool _destroyed{false};
};

#endif // WINDOW_SDL_H
