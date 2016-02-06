#include "controller.hpp"
#include <assert.h>
#include <cstring>


namespace Core {
namespace Input {


Controller::Controller()
{
  memset(m_axis, 0, sizeof(m_axis));
  memset(m_buttons, 0, sizeof(m_buttons));
}


Axis
Controller::get_axis(const uint8_t axis) const
{
  assert(axis < 2);
  return m_axis[axis];
}


bool
Controller::is_button_down(const Button::ENUM button) const
{
  return m_buttons[(size_t)button] == Button_state::button_down;
}


} // ns
} // ns