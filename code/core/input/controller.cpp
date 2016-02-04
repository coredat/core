#include "controller.hpp"
#include <assert.h>


namespace Core {
namespace Input {


Axis
Controller::get_axis(const uint8_t axis)
{
  assert(axis < 2);
  return m_axis[axis];
}


bool
Controller::is_button_down(const Button::ENUM button)
{
  return m_buttons[(size_t)button] == Button_state::button_down;
}


} // ns
} // ns