#ifndef INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76
#define INPUT_INCLUDED_98612EE1_83F4_4CC9_9D9B_757ABB3ACB76


#include "buttons.hpp"
#include "axis.hpp"
#include <stdint.h>


namespace Core {
namespace Input {


class Controller
{
public:

  Axis get_axis(const uint8_t axis);
  bool is_button_down(const Button::ENUM button);

private:

  Axis axis[2];
  Button_state buttons[16];
}; // classs


} // ns
} // ns


#endif // inc guard
