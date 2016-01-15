#include "../window.hpp"
#include "sdl_types.hpp"
#include "sdl_common.hpp"
#include <assert.h>
#include <OpenGL/gl3.h>


namespace
{
  const int32_t fullscreen_mode = SDL_WINDOW_FULLSCREEN_DESKTOP; // rather than SDL_WINDOW_FULLSCREEN
}


namespace Environment {


bool
window_create(Window *out_window,
              const uint32_t width,
              const uint32_t height,
              const bool is_fullscreen,
              const char *title,
              std::ostream *log)
{
  assert(out_window);
  
  // Setup hints (Must happen before creation of window)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  }
  
  // Create Window
  {
    const Uint32 init_flags = SDL_INIT_EVERYTHING;

    if(SDL_Init(init_flags) != 0)
    {
      SDL_Detail::check_log_error(log);
      assert(false);
      return false;
    }

    const Uint32 default_window_flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
    const Uint32 window_flags = is_fullscreen ? default_window_flags | fullscreen_mode : default_window_flags;

    out_window->window = SDL_CreateWindow(title,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          width,
                                          height,
                                          window_flags);

    if(!out_window->window)
    {
      SDL_Detail::check_log_error(log);
      assert(false);
      return false;
    }
  }
  
  // Create contexte
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 2);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    
    out_window->context = SDL_GL_CreateContext(out_window->window);
    
    if(!out_window->context)
    {
      SDL_Detail::check_log_error(log);
      assert(out_window->context);
      return false;
    }
    else
    {
      const GLubyte* vendor_str = glGetString(GL_VERSION);
      const std::string vendor_info = reinterpret_cast<const char*>(vendor_str);
      
      glClearColor(0.4, 0.2, 0.2, 1);
      glClear(GL_COLOR_BUFFER_BIT);
      
      const std::string msg = "INFO: " + vendor_info;
      SDL_Detail::log_info(msg.c_str(), log);
    }

    SDL_GL_MakeCurrent(out_window->window, out_window->context);
  }
  
  SDL_Detail::check_log_error(log);
  return true;
}


bool
window_set_resolution(Window *out_window, const uint32_t width, const uint32_t height, std::ostream *log)
{
  assert(out_window);

  int curr_width, curr_height;
  SDL_GetWindowSize(out_window->window, &curr_width, &curr_height);
  SDL_Detail::check_log_error(log);
  
  if(curr_width == width && curr_height == height)
  {
    SDL_Detail::log_info("WARNING: 'window_set_resolution()' setting resolution to existing resolution.", log);
    return false;
  }
  
  SDL_SetWindowSize(out_window->window, static_cast<int>(width), static_cast<int>(height));
  SDL_Detail::check_log_error(log);
  
  return true;
}


bool
window_set_fullscreen(Window *out_window, const bool is_fullscreen, std::ostream *log)
{
  assert(out_window);
  
  SDL_Surface* surface = SDL_GetWindowSurface(out_window->window);
  assert(surface);
  
  if(!surface)
  {
    SDL_Detail::check_log_error(log);
    SDL_Detail::log_info("ERROR: 'window_set_fullscreen()' unable to find SDL_Surface.", log);
    return false;
  }
  
  if((surface->flags & fullscreen_mode) == is_fullscreen)
  {
    SDL_Detail::log_info("WARNING: 'window_set_fullscreen()' setting fullscreen setting to existing setting.", log);
    return false;
  }

  if(SDL_SetWindowFullscreen(out_window->window, is_fullscreen ? fullscreen_mode : 0) < 0)
  {
    SDL_Detail::check_log_error(log);
    SDL_Detail::log_info("ERROR: Failed to set window fullscreen setting in 'window_set_fullscreen()'.", log);
    
    assert(false);
    return false;
  }
  
  return true;
}


void
window_destroy(Window *window, std::ostream *log)
{
}


uint32_t
window_get_width(const Window *window, std::ostream *log)
{
  assert(window);

	int w = 0;
  int h = 0;
	
	SDL_GetWindowSize(window->window, &w, &h);
  SDL_Detail::check_log_error(log);
    
  return static_cast<uint32_t>(w);
}


uint32_t
window_get_height(const Window *window, std::ostream *log)
{
  assert(window);

	int w = 0;
  int h = 0;
	
	SDL_GetWindowSize(window->window, &w, &h);
  SDL_Detail::check_log_error(log);
    
  return static_cast<uint32_t>(h);
}


void
window_think(const Window *window, std::ostream *log)
{
  SDL_GL_SwapWindow(window->window);
}


void
window_callbacks(const Window *window, const std::function<void()> &callback, std::ostream *log)
{
}


} // ns