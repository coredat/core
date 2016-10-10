#include <core/context/context_setup.hpp>
#include <core/context/detail/context_sdl_impl.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/context_data/input_pool.hpp>
#include <utilities/logging.hpp>
#include <common/error_strings.hpp>
#include <stdatomic.h>


namespace {

/*
  SDL has two fullscreen modes.
  SDL_WINDOW_FULLSCREEN_DESKTOP - uses current resolution of desktop
  SDL_WINDOW_FULLSCREEN
*/
constexpr Uint32 fullscreen_mode = SDL_WINDOW_FULLSCREEN;


void
register_sdl_callbacks(void *context, void *input)
{
  // Window callback
  Sdl::event_add_callback([](const SDL_Event *evt, void *self)
  {
    Core_detail::Sdl_context *impl = reinterpret_cast<Core_detail::Sdl_context*>(self);
    
    LOG_TODO_ONCE("Move this to a transform or into sdl_backend system");
    
    switch(evt->type)
    {
      // Time to quit.
      case(SDL_QUIT):
        impl->close();
        break;
        
      case(SDL_WINDOWEVENT):
      {
        switch(evt->window.event)
        {
          case(SDL_WINDOWEVENT_RESIZED):
          {
            LOG_TODO("This event could be called when transitioning to or from a retina monitor. Do I need to regenerate fbo's?");
            break;
          }
          case(SDL_WINDOWEVENT_FOCUS_LOST):
          {
            LOG_TODO("Should button events be reset? Do We still get other events?");
            break;
          }
          case(SDL_WINDOWEVENT_FOCUS_GAINED):
          {
            LOG_TODO("Do we need to reinitialize anything here?");
            break;
          }
        }
        break;
      }
    }
  },
  Sdl::context_slot(),
  context);

  // Input callbacks
  Sdl::event_add_callback([](const SDL_Event *evt, void *self)
  {
    Context_data::Input_pool *input_data = reinterpret_cast<Context_data::Input_pool*>(self);

    if(input_data)
    {
      Sdl::process_input_messages(evt, input_data);
    }
  },
  Sdl::input_slot(),
  input);
}

  
} // anon ns


namespace Core_detail {


Sdl_context::Sdl_context()
{
  // Basically invalid object.
}


Sdl_context::Sdl_context(const uint32_t width,
                         const uint32_t height,
                         const bool is_fullscreen,
                         const char *title,
                         const Core::Context_setup &settings,
                         std::shared_ptr<Core::Context_detail::Data> data)
: m_is_open(false)
, m_window(nullptr)
, m_context(nullptr)
, m_data(data)
{
  // Setup hints (Must happen before creation of window)
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  }
  
  // SDL Hints
  {
    #ifdef __APPLE__
    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1"); // https://forums.libsdl.org/viewtopic.php?p=51127&sid=570a47d0f562cc0b1d4c91b7712c663f
    #endif
  }

  
  // Create Window
  {
    const Uint32 init_flags = SDL_INIT_EVERYTHING;

    if(SDL_Init(init_flags) != 0)
    {
      assert(false);
      LOG_FATAL(Error_string::generic_sdl_fail());
    }
    
    const Uint32 window_high_dpi      = settings.high_dpi_support ? SDL_WINDOW_ALLOW_HIGHDPI : 0;
    const Uint32 default_window_flags = window_high_dpi | SDL_WINDOW_OPENGL;
    const Uint32 window_flags         = is_fullscreen ? default_window_flags | fullscreen_mode : default_window_flags;
    const Uint32 display_startup      = math::min(settings.monitor_preference, math::to_uint(SDL_GetNumVideoDisplays()));

    m_window = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_CENTERED_DISPLAY(display_startup),
                                SDL_WINDOWPOS_CENTERED_DISPLAY(display_startup),
                                width,
                                height,
                                window_flags);

    if(!m_window)
    {
      assert(false);
      LOG_FATAL("Failed to create an SDL Window");
    }
    else
    {
      m_is_open = true;
    }
  }
  
  // Create context
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,              Graphics_api::pixel_format_red_bits(settings.backbuffer_fmt));
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,            Graphics_api::pixel_format_green_bits(settings.backbuffer_fmt));
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,             Graphics_api::pixel_format_blue_bits(settings.backbuffer_fmt));
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,            Graphics_api::pixel_format_alpha_bits(settings.backbuffer_fmt));
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,            24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,          1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,    0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,    0);

    m_context = SDL_GL_CreateContext(m_window);
    
    if(!m_context)
    {
      assert(m_context);
      LOG_FATAL(Error_string::generic_ogl_fail());
    }
    
    SDL_GL_MakeCurrent(m_window, m_context);
    SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0); // Vsync

    // We initialize GLEW first.
    #ifdef _WIN32
    glewExperimental = GL_TRUE;
    const  GLenum err = glewInit();
    if (err != GLEW_OK)
    {
      LOG_ERROR(Error_string::generic_ogl_fail());
    }
    #endif
  }
  
  register_sdl_callbacks(this, m_data->input_pool);
}


Sdl_context::~Sdl_context()
{
  if(m_window)
  {
    SDL_Quit();
    
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }
  
  if(m_context)
  {
    SDL_GL_DeleteContext(m_context);
    m_context = nullptr;
  }
}


Sdl_context::Sdl_context(Sdl_context &&other)
{
  m_is_open = other.m_is_open;
  m_window  = other.m_window;
  m_context = other.m_context;
  m_data    = other.m_data;
  
  other.m_is_open = false;
  other.m_window  = nullptr;
  other.m_context = nullptr;
  other.m_data    = nullptr;
  
  register_sdl_callbacks(this, m_data->input_pool);
}


Sdl_context&
Sdl_context::operator=(Sdl_context &&other)
{
  m_is_open = other.m_is_open;
  m_window  = other.m_window;
  m_context = other.m_context;
  m_data    = other.m_data;
  
  other.m_is_open = false;
  other.m_window  = nullptr;
  other.m_context = nullptr;
  other.m_data    = nullptr;
  
  register_sdl_callbacks(this, m_data->input_pool);
  
  return *this;
}


bool
Sdl_context::is_open() const
{
  return m_is_open;
}


void
Sdl_context::close()
{
  m_is_open = false;
}


void
Sdl_context::set_resolution(const uint32_t width,
                            const uint32_t height)
{
  int curr_width, curr_height;
  SDL_GetWindowSize(m_window, &curr_width, &curr_height);
  
  if(curr_height == height && curr_width == width)
  {
    return;
  }
  
  SDL_SetWindowSize(m_window, static_cast<int>(width), static_cast<int>(height));
}


uint32_t
Sdl_context::get_width() const
{
  int w = 0;
  int h = 0;

  //	SDL_GetWindowSize(m_impl->window, &w, &h);
  SDL_GL_GetDrawableSize(m_window, &w, &h);

  return static_cast<uint32_t>(w);
}


uint32_t
Sdl_context::get_height() const
{
	int w = 0;
  int h = 0;
	
//	SDL_GetWindowSize(m_impl->window, &w, &h);
  SDL_GL_GetDrawableSize(m_window, &w, &h);
  
  return static_cast<uint32_t>(h);
}


void
Sdl_context::set_fullscreen(const bool set)
{
  if(is_fullscreen() == set)
  {
    return;
  }
  
  if(SDL_SetWindowFullscreen(m_window, set ? fullscreen_mode : 0) < 0)
  {
    assert(false);
    LOG_ERROR(Error_string::generic_sdl_fail())
    return;
  }
}


bool
Sdl_context::is_fullscreen() const
{
  const int flags = SDL_GetWindowFlags(m_window);

  return !!(flags & fullscreen_mode);
}


bool
Sdl_context::process()
{
  if(m_is_open)
  {
    auto num_controllers = SDL_NumJoysticks();
    
    for(uint32_t i = 0; i < num_controllers; ++i)
    {
      SDL_GameControllerOpen(i);
    }

    // Flip buffer and process events.
    SDL_GL_SwapWindow(m_window);

    Sdl::event_process();
  }
  
  return is_open();
}


uint32_t
Sdl_context::get_display_index() const
{
  return SDL_GetWindowDisplayIndex(m_window);
}


const char *
Sdl_context::get_title() const
{
  return SDL_GetWindowTitle(m_window);
}
  

void
Sdl_context::set_title(const char *title)
{
  SDL_SetWindowTitle(m_window, title);
}


SDL_Window*
Sdl_context::get_sdl_window() const
{
  return m_window;
}


SDL_GLContext
Sdl_context::get_sdl_gl_context() const
{
  return m_context;
}


} // ns