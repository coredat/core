#ifndef INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76
#define INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76


#include <core/common/core_fwd.hpp>
#include <core/input/axis.hpp>


namespace Core {
namespace Input {


/*!
  Toggle if the mouse is captured.
*/
void
mouse_set_capture(Core::Context &context, const bool capture);


/*!
  Returns true if the mouse has been captured.
*/
bool
mouse_is_capture(Core::Context &context);


/*!
  Returns the mouse coordinates.
*/
Axis
mouse_get_coordinates(Core::Context &context);


/*!
  Returns true if the controller have been used.
*/
bool
controllers_are_in_use(Core::Context &context);


/*!
  Get keyboard stream
*/
using Keyboard_stream_fn = void(*)(const uintptr_t user_data, const char *);

void
keyboard_stream(Core::Context &context,
                const uintptr_t user_data,
                const Keyboard_stream_fn &stream_fn);


} // ns
} // ns


#endif // inc guard
