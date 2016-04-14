#ifndef INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76
#define INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76


#include <core/input/controller.hpp>
#include <core/context/context_fwd.hpp>
#include <stdint.h>


namespace Core {
namespace Input {


namespace Player {
enum ENUM {

  one = 0,
  two,
  three,
  four,

}; // enum
} // ns


void
mouse_set_capture(Core::Context &context, const bool capture);


bool
mouse_is_capture(Core::Context &context);


} // ns
} // ns


#endif // inc guard
