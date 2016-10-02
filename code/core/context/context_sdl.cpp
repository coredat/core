
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
#include <transformations/input/frame_update.hpp>
#include <math/general/general.hpp>
#include <assert.h>

#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_impl_sdl_gl3.h>


namespace {

  /*
    SDL has two fullscreen modes.
    SDL_WINDOW_FULLSCREEN_DESKTOP - uses current resolution of desktop
    SDL_WINDOW_FULLSCREEN
  */
  constexpr Uint32 fullscreen_mode = SDL_WINDOW_FULLSCREEN;
  
  // only one instance allowed in an application lifetime.
  // This could change, just right now no need.
  atomic_bool instance_created(false);
  
  Context_data::Input_pool core_input;

} // anon ns


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

    m_impl->window = SDL_CreateWindow(title,
                                      SDL_WINDOWPOS_CENTERED_DISPLAY(display_startup),
                                      SDL_WINDOWPOS_CENTERED_DISPLAY(display_startup),
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

  // Core data
  Context_data::input_data_init(&core_input);
  m_impl->context_data->input_pool = &core_input;


  // Window callback
  Sdl::event_add_callback([](const SDL_Event *evt, void *self)
  {
    Context::Impl *impl = reinterpret_cast<Context::Impl*>(self);
    
    LOG_TODO_ONCE("Move this to a transform or into sdl_backend system");
    
    switch(evt->type)
    {
      // Time to quit.
      case(SDL_QUIT):
        impl->is_open = false;
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
  m_impl.get());


  // Input callbacks
  Sdl::event_add_callback([](const SDL_Event *evt, void *self)
  {
    Context_data::Input_pool *input_data = reinterpret_cast<Context_data::Input_pool*>(self);

    if(input_data)
    {
      Sdl::process_input_messages(evt, input_data);
    }
  },
  &core_input);
  //nullptr);
  //m_impl->context_data->input_pool);
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
Context::set_resolution(const uint32_t width, const uint32_t height)
{
  assert(m_impl);

  int curr_width, curr_height;
  SDL_GetWindowSize(m_impl->window, &curr_width, &curr_height);
  
  if(curr_height == height && curr_width == width)
  {
    return;
  }
  
  SDL_SetWindowSize(m_impl->window, static_cast<int>(width), static_cast<int>(height));
}


bool
Context::is_fullscreen() const
{
  assert(m_impl);
  assert(m_impl->window);
  
  const int flags = SDL_GetWindowFlags(m_impl->window);
  
  return !!(flags & fullscreen_mode);
}


void
Context::set_fullscreen(const bool set_fullscreen)
{
  assert(m_impl);
  
  if(is_fullscreen() == set_fullscreen)
  {
    return;
  }
  
  if(SDL_SetWindowFullscreen(m_impl->window, set_fullscreen ? fullscreen_mode : 0) < 0)
  {
    assert(false);
    LOG_ERROR(Error_string::generic_sdl_fail())
    return;
  }
}


uint32_t
Context::get_display() const
{
  assert(m_impl);
  
  return SDL_GetWindowDisplayIndex(m_impl->window);
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
  if(!m_impl->is_open)
  {
    return true;
  }

  assert(m_impl);
  #ifdef CORE_DEBUG_MENU
  ImGui::Render();
  #endif
  
  auto num_controllers = SDL_NumJoysticks();
  
  for(uint32_t i = 0; i < num_controllers; ++i)
  {
    SDL_GameControllerOpen(i);
  }

  // Flip buffer and process events.
  SDL_GL_SwapWindow(m_impl->window);
  ImGui_ImplSdlGL3_NewFrame(m_impl->window);
  
  Input_utils::update_input_state(m_impl->context_data->input_pool);
//  Sdl::update_keyboard_controller(&(m_impl->context_data->input_pool->controllers[0]));
  
  // Reset the memory pool.
  Memory::scratch_reset();
  Sdl::event_process();
  
  return m_impl->is_open;
}


Context::operator bool() const
{
  return is_open();
}


void
Context::close()
{
  m_impl->is_open = false;
  SDL_Quit();
}


std::shared_ptr<const Context_detail::Data>
Context::get_context_data() const
{
  assert(m_impl);
  return m_impl->context_data;
}


} // ns

#endif // define CORE_USE_SDL