#include <core/input/input.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/context_data/input_pool.hpp>
#include <systems/sdl_backend/sdl_input.hpp>


namespace {


uintptr_t input_user_data = 0;
//Sdl::Text_input_stream_fn sdl_input_stream = nullptr;
Core::Input::Keyboard_stream_fn keyboard_stream_callback = nullptr;

inline void
sdl_input_stream(const uint8_t *text, const size_t size)
{
  if(keyboard_stream_callback)
  {
    keyboard_stream_callback(input_user_data, text, size);
  }
}


} // anon ns


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
  return context.get_context_data()->input_pool->mice[0].position;
}


bool
controllers_are_in_use(Core::Context &context)
{
  return context.get_context_data()->input_pool->controllers_touched;
}


void
keyboard_stream(Core::Context &context,
                const uintptr_t user_data,
                const Keyboard_stream_fn &stream_fn)
{
  keyboard_stream_callback = stream_fn;
  
  Sdl::get_text_input_stream(sdl_input_stream);
}


} // ns
} // ns
