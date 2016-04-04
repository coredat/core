#include "controller.hpp"
#include <assert.h>
#include <cstring>


namespace Core {
namespace Input {


struct Controller::Impl
{
  void *controller_data = nullptr;
};


Controller::Controller(const Core::Context &ctx, const int player)
: m_impl(new Impl)
{
  memset(m_axis, 0, sizeof(m_axis));
  memset(m_buttons, 0, sizeof(m_buttons));
}


Controller::Controller(const Controller &other)
{
}


Controller::Controller(Controller &&other)
{
}

  
Controller&
Controller::operator=(const Controller &other)
{
  return *this;
}


Controller&
Controller::operator=(Controller &&other)
{
  return *this;
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
  return m_buttons[(uint32_t)button] == Button_state::down;
}


} // ns
} // ns