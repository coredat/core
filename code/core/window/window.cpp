#include <core/window/window.hpp>
#include <data/world_data/window_data.hpp>
#include <data/world_data/world.hpp>
#include <data/world_data/world_pools.hpp>
#include <assert.h>


namespace Core {


struct Window::Impl
{
};


namespace
{
  World_data::Application_window *
  get_window()
  {
    World_data::World *world = World_data::get_world();
    assert(world);
    
    World_data::Application_window *window = world->app_window;
    assert(window);
    
    return window;
  }
}


Window::Window(const uint32_t width,
               const uint32_t height,
               const bool is_fullscreen,
               const char *title)
: m_impl(new Window::Impl)
{
  
}


Window::~Window()
{
  World_data::application_window_close(get_window());
}


Window::Window(Window &&other)
{
}


Window& Window::operator=(Window &&other)
{
  return *this;
}


uint32_t
Window::get_width() const
{
  return 0;
}

uint32_t
Window::get_height() const
{
  return 0;
}


bool
Window::is_fullscreen() const
{
  return false;
}


const char *
Window::get_title() const
{
  return "";
}


bool
Window::is_open() const
{
  return true;
}


Window::operator bool() const
{
  return is_open();
}


} // ns