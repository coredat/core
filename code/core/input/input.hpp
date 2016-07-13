#ifndef INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76
#define INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76


#include <core/common/core_fwd.hpp>


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


} // ns
} // ns


#endif // inc guard
