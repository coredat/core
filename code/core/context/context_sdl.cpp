
/*
  TODO:
  Split SDL Out need to trim this stuff outta here.
*/

#define CORE_USE_SDL // Only have sdl at this point

#ifdef CORE_USE_SDL

#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>
#include <graphics_api/ogl/ogl_common.hpp>
#include <systems/sdl_backend/sdl_message_loop.hpp>
#include <systems/sdl_backend/sdl_input.hpp>
#include <data/global_data/memory_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/conversion.hpp>
#include <utilities/threading.hpp>
#include <common/error_strings.hpp>
#include <assert.h>

#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_impl_sdl_gl3.h>


namespace
{
  // Rather than SDL_WINDOW_FULLSCREEN - aka fake fullscreen.
  constexpr Uint32 fullscreen_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
  
  // only one instance allowed in an application lifetime.
  // This could change, just right now no need.
  atomic_bool instance_created(false);
  
  Context_data::Input_pool core_input;
}


namespace Core {


/*
  Holds the SDL impl details.
*/
struct
Context::Impl
{
  bool is_open = true;
  SDL_Window *window = nullptr;
  SDL_GLContext context = nullptr;
  std::shared_ptr<Context_detail::Data> context_data = std::make_shared<Context_detail::Data>();
};


Context::Context(const uint32_t width,
                 const uint32_t height,
                 const bool is_fullscreen,
                 const char *title,
                 Context_setup settings)
: m_impl(new Context::Impl{})
{
  if(instance_created)
  {
    assert(false); // For the moment we only support making one instance per life of an application.
    
    return;
  }
  else
  {
    instance_created = true;
  }
  
  // Initialize memory
  {
    constexpr size_t pool_bytes    = util::convert_mb_to_bytes(64);
    constexpr size_t scratch_bytes = util::convert_mb_to_bytes(64);
    
    Memory::memory_initialize(pool_bytes, scratch_bytes);
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
      LOG_FATAL(Error_string::generic_sdl_fail());
    }

    const Uint32 default_window_flags = /*SDL_WINDOW_ALLOW_HIGHDPI |*/ SDL_WINDOW_OPENGL;
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

    m_impl->context = SDL_GL_CreateContext(m_impl->window);
    
    if(!m_impl->context)
    {
      assert(m_impl->context);
      LOG_FATAL(Error_string::generic_ogl_fail());
    }
    
    SDL_GL_MakeCurrent(m_impl->window, m_impl->context);
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
  
  // Initialize the graphics api
  Graphics_api::initialize();
  // Clear is important, some platforms will render junk for a frame.
  Graphics_api::clear(Graphics_api::Clear_flag::color | Graphics_api::Clear_flag::depth);
  
//  SDL_StartTextInput();
  
  #ifdef CORE_DEBUG_MENU
  ImGui_ImplSdlGL3_Init(m_impl->window);
  ImGui_ImplSdlGL3_NewFrame(m_impl->window);
  #endif

  // Window callback
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
  
  // Input callback
  Sdl::event_add_callback([](const SDL_Event *evt, void *self)
  {
    // Not currently used.
  },
  m_impl.get());

  // Core data
  Context_data::input_data_init(&core_input);
  m_impl->context_data->input_pool = &core_input;
}


Context::~Context()
{
  #ifdef CORE_DEBUG_MENU
  ImGui_ImplSdlGL3_Shutdown();
  #endif
  
  instance_created = false;
}


Context::Context(Context &&other)
: m_impl(std::move(other.m_impl))
{
  m_impl->context = other.m_impl->context;
  other.m_impl->context = nullptr;
  
  m_impl->context_data = other.m_impl->context_data;
  other.m_impl->context_data = nullptr;
  
  m_impl->is_open = other.m_impl->is_open;
  other.m_impl->is_open = false;
  
  m_impl->window = other.m_impl->window;
  other.m_impl->window = nullptr;
}


Context&
Context::operator=(Context &&other)
{
  m_impl->context = other.m_impl->context;
  other.m_impl->context = nullptr;
  
  m_impl->context_data = other.m_impl->context_data;
  other.m_impl->context_data = nullptr;
  
  m_impl->is_open = other.m_impl->is_open;
  other.m_impl->is_open = false;
  
  m_impl->window = other.m_impl->window;
  other.m_impl->window = nullptr;
  
  return *this;
}


uint32_t
Context::get_width() const
{
  assert(m_impl->window);

	int w = 0;
  int h = 0;
	
//	SDL_GetWindowSize(m_impl->window, &w, &h);
  SDL_GL_GetDrawableSize(m_impl->window, &w, &h);
  
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
	
//	SDL_GetWindowSize(m_impl->window, &w, &h);
  SDL_GL_GetDrawableSize(m_impl->window, &w, &h);
  
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
    LOG_ERROR(Error_string::generic_sdl_fail())
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
    LOG_ERROR(Error_string::generic_sdl_fail())
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
    LOG_ERROR(Error_string::generic_sdl_fail())
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
  #ifdef CORE_DEBUG_MENU
  ImGui::Render();
  #endif
  
  // Reset the memory pool.
  Memory::scratch_reset();
  Sdl::event_process();

  // Flip buffer and process events.
  SDL_GL_SwapWindow(m_impl->window);
  ImGui_ImplSdlGL3_NewFrame(m_impl->window);
  
  const uint32_t num_controllers = SDL_NumJoysticks();
  
  if(num_controllers)
  {
    Sdl::update_gamepad_controller(&(m_impl->context_data->input_pool->controllers[0]), 0);
    
    if(num_controllers > 1)
    {
      Sdl::update_gamepad_controller(&(m_impl->context_data->input_pool->controllers[1]), 1);
    }
    
    if(num_controllers > 2)
    {
      Sdl::update_gamepad_controller(&(m_impl->context_data->input_pool->controllers[2]), 2);
    }

    if(num_controllers > 3)
    {
      Sdl::update_gamepad_controller(&(m_impl->context_data->input_pool->controllers[3]), 3);
    }
  }
  else
  {
    Sdl::update_keyboard_controller(&(m_impl->context_data->input_pool->controllers[0]));
  }
  
  return m_impl->is_open;
}


Context::operator bool() const
{
  return is_open();
}


std::shared_ptr<const Context_detail::Data>
Context::get_context_data() const
{
  assert(m_impl);
  return m_impl->context_data;
}


} // ns

#endif // define CORE_USE_SDL