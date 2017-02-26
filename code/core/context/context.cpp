#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>
#include <graphics_api/ogl/ogl_common.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/convert.hpp>
#include <common/error_strings.hpp>
#include <transformations/input/frame_update.hpp>
#include <math/general/general.hpp>
#include <assert.h>

#include <core/context/detail/context_sdl_impl.hpp>


#ifdef CORE_DEBUG_MENU
#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_impl_sdl_gl3.h>
#endif


namespace {
  
  // only one instance allowed in an application lifetime.
  // This could change, just right now no need.
  std::atomic_bool instance_created(false);
  
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
    constexpr size_t pool_bytes    = lib::convert::mb_to_bytes(128);
    constexpr size_t scratch_bytes = lib::convert::mb_to_bytes(128);
    
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
  ImGui_ImplSdlGL3_NewFrame(m_impl->impl_context.get_sdl_window());

  Graphics_api::reset();
  #endif
  
  Input_utils::update_input_state(m_impl->context_data->input_pool);
  
  // Reset the memory pool.
  Memory::scratch_reset();
  
  m_impl->impl_context.process();
  
  return true;
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
