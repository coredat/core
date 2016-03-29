#define CORE_USE_SDL // Only have sdl at this point

#ifdef CORE_USE_SDL

#include <core/context/context.hpp>
#include <core/context/context_graphics_api.hpp>
#include <data/world_data/window_data.hpp>
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <systems/sdl_backend/sdl_message_loop.hpp>
#include <assert.h>


namespace
{
  constexpr Uint32 fullscreen_mode = SDL_WINDOW_FULLSCREEN_DESKTOP; // rather than SDL_WINDOW_FULLSCREEN
}


namespace Core {


struct Context::Impl
{
  bool is_open = true;
  SDL_Window *window    = nullptr;
  SDL_GLContext context = nullptr;
};


Context::Context(const Graphics_api gfx_api,
                 const uint32_t width,
                 const uint32_t height,
                 const bool is_fullscreen,
                 const char *title)
: m_impl(new Context::Impl)
{
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
    }
  }


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
{

}


Context&
Context::operator=(Context &&other)
{
  return *this;
}


uint32_t
Context::get_width() const
{
  return 0;
}

uint32_t
Context::get_height() const
{
  return 0;
}


bool
Context::is_fullscreen() const
{
  return false;
}


const char *
Context::get_title() const
{
  return "";
}


bool
Context::is_open() const
{
  return m_impl->is_open;
}


Context::operator bool() const
{
  return is_open();
}


} // ns

#endif // define CORE_USE_SDL