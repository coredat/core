#include <core/input/input.hpp>
#include <core/context/context.hpp>
#include <data/context_data/input_pool.hpp>
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


Axis
mouse_get_coordinates(Core::Context &context)
{
  context.get_context_data();
}


} // ns
} // ns
