#include <core/window/window.hpp>


namespace Core {


struct Window::Impl
{
};


Window::Window(const uint32_t width,
               const uint32_t height,
               const bool is_fullscreen,
               const char *title)
: m_impl(new Window::Impl)
{
}


Window::~Window()
{
}


Window::Window(Window &&other)
{
}


Window& Window::operator=(Window &&other)
{
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


} // ns