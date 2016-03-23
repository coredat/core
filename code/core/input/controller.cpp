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
  return m_buttons[(uint32_t)button] == Button_state::down;
}


void
Controller::serialize(uint8_t out_data[16])
{
 // uint32_t offset = 0;
  
  // Copy axis.

//  memcpy(&out_data[offset], &m_axis[0].x, sizeof(float));
//  offset += sizeof(float);
//
//  memcpy(&out_data[offset], &m_axis[0].y, sizeof(float));
//  offset += sizeof(float);
//
//  memcpy(&out_data[offset], &m_axis[1].x, sizeof(float));
//  offset += sizeof(float);
//
//  memcpy(&out_data[offset], &m_axis[1].y, sizeof(float));
//  offset += sizeof(float);

  // Buttons

  // Not doing buttons just yet.
}


} // ns
} // ns