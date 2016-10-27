#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>
#include <graphics_api/ogl/ogl_common.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/conversion.hpp>
#include <utilities/threading.hpp>
#include <common/error_strings.hpp>
#include <transformations/input/frame_update.hpp>
#include <math/general/general.hpp>
#include <assert.h>

#include <core/context/detail/context_sdl_impl.hpp>


#ifdef CORE_DEBUG_MENU
#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_impl_sdl_gl3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include <3rdparty/nuklear/nuklear.h>
#include <3rdparty/nuklear/nuklear_sdl_gl3.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
#endif


namespace {
  
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
  std::shared_ptr<Context_detail::Data> context_data;
  Core_detail::Sdl_context impl_context;
};


Context::Context(const uint32_t width,
                 const uint32_t height,
                 const bool is_fullscreen,
                 const char *title,
                 Context_setup settings)
: m_impl(new Context::Impl)
{
  if(instance_created)
  {
    // For the moment we only support making one instance per life of an application.
    assert(false);
    
    return;
  }
  else
  {
    m_impl->context_data.reset(new Context_detail::Data{&core_input});
    m_impl->impl_context = Core_detail::Sdl_context(width,
                                                    height,
                                                    is_fullscreen,
                                                    title,
                                                    settings,
                                                    m_impl->context_data);

    instance_created = true;
  }
  
  // Initialize memory
  {
    constexpr size_t pool_bytes    = util::convert_mb_to_bytes(64);
    constexpr size_t scratch_bytes = util::convert_mb_to_bytes(64);
    
    Memory::memory_initialize(pool_bytes, scratch_bytes);
  }
  
  
  // Core data
  Context_data::input_data_init(m_impl->context_data->input_pool);
  
  // Initialize the graphics api
  Graphics_api::initialize();
  // Clear is important, some platforms will render junk for a frame.
  Graphics_api::clear(Graphics_api::Clear_flag::color | Graphics_api::Clear_flag::depth);
  
//  SDL_StartTextInput();
  
  #ifdef CORE_DEBUG_MENU
  ImGui_ImplSdlGL3_Init(m_impl->impl_context.get_sdl_window());
  ImGui_ImplSdlGL3_NewFrame(m_impl->impl_context.get_sdl_window());
  
  if(!m_impl->context_data->ctx)
  {
    m_impl->context_data->ctx = nk_sdl_init(m_impl->impl_context.get_sdl_window());
    
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
  }
  #endif
  
  // After all HW has been init, init the context data.
  {
    Data::init_context_data(Data::Context_data_setup{});
  }
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

}


Context&
Context::operator=(Context &&other)
{
  return *this;
}


uint32_t
Context::get_width() const
{
  assert(m_impl);
  return m_impl->impl_context.get_width();
}


uint32_t
Context::get_height() const
{
  assert(m_impl);
  return m_impl->impl_context.get_height();
}


void
Context::set_resolution(const uint32_t width, const uint32_t height)
{
  assert(m_impl);
  m_impl->impl_context.set_resolution(width, height);
}


bool
Context::is_fullscreen() const
{
  assert(m_impl);
  return m_impl->impl_context.is_fullscreen();
}


void
Context::set_fullscreen(const bool set_fullscreen)
{
  assert(m_impl);
  m_impl->impl_context.set_fullscreen(set_fullscreen);
}


uint32_t
Context::get_display() const
{
  assert(m_impl);
  return m_impl->impl_context.get_display_index();
}


const char *
Context::get_title() const
{
  assert(m_impl);
  return m_impl->impl_context.get_title();
}


void
Context::set_title(const char * title)
{
  assert(m_impl && title);
  m_impl->impl_context.set_title(title);
}


bool
Context::is_open() const
{
  if(!m_impl->impl_context.is_open())
  {
    return false;
  }

  assert(m_impl);
  #ifdef CORE_DEBUG_MENU
  ImGui::Render();
  Graphics_api::reset();
  
  
  nk_sdl_render(NK_ANTI_ALIASING_OFF, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

  Graphics_api::reset();
  #endif
  
  ImGui_ImplSdlGL3_NewFrame(m_impl->impl_context.get_sdl_window());
  
  Input_utils::update_input_state(m_impl->context_data->input_pool);
  
  // Reset the memory pool.
  Memory::scratch_reset();
  
  nk_input_begin(m_impl->context_data->ctx);
  return m_impl->impl_context.process();
  nk_input_end(m_impl->context_data->ctx);
}


Context::operator bool() const
{
  return is_open();
}


void
Context::close()
{
  m_impl->impl_context.close();
}


std::shared_ptr<const Context_detail::Data>
Context::get_context_data() const
{
  assert(m_impl);
  return m_impl->context_data;
}


void*
Context::get_device_context()
{
  assert(m_impl);
  return m_impl->context_data->ctx;
}


} // ns