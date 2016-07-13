#include <core/input/input.hpp>
#include <systems/sdl_backend/sdl_input.hpp>


namespace Core {
namespace Input {


void
mouse_set_capture(Core::Context &context, const bool capture)
{
  Sdl::capture_mouse(capture);
}


bool
mouse_is_capture(Core::Context &context)
{
  return Sdl::is_mouse_captured();
}


} // ns
} // ns
