
/*
  TODO:
  This needs better logging.
*/

#define CORE_USE_SDL // Only have sdl at this point

#ifdef CORE_USE_SDL

#include <core/context/context.hpp>
#include <graphics_api/initialize.hpp>
#include <systems/sdl_backend/sdl_message_loop.hpp>
#include <stdatomic.h>
#include <assert.h>
#include <utilities/logging.hpp>


namespace
{
  // Rather than SDL_WINDOW_FULLSCREEN - aka fake fullscreen.
  constexpr Uint32 fullscreen_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
  
  // only one instance allowed in an application lifetime.
  // This could change, just right now no need.
  atomic_bool instance_created(false);
}


namespace Core {


/*
  Holds the SDL impl details.
*/
struct
Context::Impl
{
  bool is_open               = true;
  SDL_Window *window         = nullptr;
  SDL_GLContext context      = nullptr;
};


Context::Context(const uint32_t width,
                 const uint32_t height,
                 const bool is_fullscreen,
                 const char *title)
: m_impl(new Context::Impl
{
  true,
  nullptr,
  nullptr,
})
{
  if(instance_created)
  {
    assert(false);
    return;
  }
  else
  {
    instance_created = true;
  }

  // Setup hints (Must happen before creation of window)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  }
  
  // Create Window
  {
    const Uint32 init_flags = SDL_INIT_EVERYTHING;

    if(SDL_Init(init_flags) != 0)
    {
      assert(false);
      LOG_FATAL("Failed to initialize SDL");
    }

    const Uint32 default_window_flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
    const Uint32 window_flags = is_fullscreen ? default_window_flags | fullscreen_mode : default_window_flags;

    m_impl->window = SDL_CreateWindow(title,
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      width,
                                      height,
                                      window_flags);

    if(!m_impl->window)
    {
      assert(false);
      LOG_FATAL("Failed to create an SDL Window");
    }
  }
  
  // Create context
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
    
    m_impl->context = SDL_GL_CreateContext(m_impl->window);
    
    if(!m_impl->context)
    {
      assert(m_impl->context);
      LOG_FATAL("Failed to create an OpenGL Context");
    }
  }
  
  // Initialize the graphics api
  ::Graphics_api::initialize();

  // Subscribe to the SDL event callback.
  Sdl::event_add_callback([](const SDL_Event *evt, void *self)
  {
    Context::Impl *impl = reinterpret_cast<Context::Impl*>(self);
    
    switch(evt->type)
    {
      // Time to quit.
      case(SDL_QUIT):
        impl->is_open = false;
        break;
    }
  },
  m_impl.get());
}


Context::~Context()
{

}


Context::Context(Context &&other)
: m_impl(std::move(other.m_impl))
{
}


Context&
Context::operator=(Context &&other)
{
  this->m_impl = std::move(other.m_impl);
  return *this;
}


uint32_t
Context::get_width() const
{
  assert(m_impl->window);

	int w = 0;
  int h = 0;
	
	SDL_GetWindowSize(m_impl->window, &w, &h);
  
  return static_cast<uint32_t>(w);
}


void
Context::set_width(const uint32_t width)
{
  assert(m_impl);

  int curr_width, curr_height;
  SDL_GetWindowSize(m_impl->window, &curr_width, &curr_height);
  
  if(curr_width == width)
  {
    return;
  }
  
  SDL_SetWindowSize(m_impl->window, static_cast<int>(width), curr_height);
}


uint32_t
Context::get_height() const
{
  assert(m_impl->window);

	int w = 0;
  int h = 0;
	
	SDL_GetWindowSize(m_impl->window, &w, &h);
  
  return static_cast<uint32_t>(h);
}


void
Context::set_height(const uint32_t height)
{
  assert(m_impl);

  int curr_width, curr_height;
  SDL_GetWindowSize(m_impl->window, &curr_width, &curr_height);
  
  if(curr_height == height)
  {
    return;
  }
  
  SDL_SetWindowSize(m_impl->window, curr_width, static_cast<int>(height));
}


bool
Context::is_fullscreen() const
{
  assert(m_impl);
  
  SDL_Surface* surface = SDL_GetWindowSurface(m_impl->window);
  assert(surface);
  
  if(!surface)
  {
    assert(false);
    LOG_ERROR("Failed to find SDL Surface")
    return false;
  }
  
  return !!(surface->flags & fullscreen_mode);
}


void
Context::set_fullscreen(const bool is_fullscreen)
{
  assert(m_impl);
  
  SDL_Surface* surface = SDL_GetWindowSurface(m_impl->window);
  assert(surface);
  
  if(!surface)
  {
    assert(false);
    LOG_ERROR("Failed to find SDL Surface")
    return;
  }
  
  if((surface->flags & fullscreen_mode) == is_fullscreen)
  {
    assert(false);
    return;
  }

  if(SDL_SetWindowFullscreen(m_impl->window, is_fullscreen ? fullscreen_mode : 0) < 0)
  {
    assert(false);
    LOG_ERROR("Failed to find set fullscreen")
    return;
  }
}


const char *
Context::get_title() const
{
  assert(m_impl);

  return SDL_GetWindowTitle(m_impl->window);
}


void
Context::set_title(const char * title)
{
  assert(m_impl);
  
  SDL_SetWindowTitle(m_impl->window, title);
}


bool
Context::is_open() const
{
  assert(m_impl);
  
  SDL_GL_SwapWindow(m_impl->window);
  Sdl::event_process();
  
  return m_impl->is_open;
}


Context::operator bool() const
{
  return is_open();
}


} // ns

#endif // define CORE_USE_SDL