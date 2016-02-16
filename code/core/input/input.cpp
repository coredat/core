#include "input.hpp"
#include "detail/controller_members.hpp"
#include <data/core_data/core_data.hpp>
#include <stddef.h>
#include <string.h>


namespace
{
  
}


namespace Core {
namespace Input {


Controller
get_controller(const Player::ENUM player)
{
  auto core_data  = Core_data::get_core_data();
  auto input_data = Core_data::get_input_data(core_data);

  struct Private_access
  {
    INPUT_CONTROLLER_MEMBERS
  };

  Controller controller;
  Private_access *access = reinterpret_cast<Private_access*>(&controller);
  
  access->m_axis[0] = input_data->controllers[0].axis[0];
  access->m_axis[1] = input_data->controllers[0].axis[1];
  memcpy(&access->m_buttons[0], &input_data->controllers[0].buttons[0], sizeof(access->m_buttons));

  return controller;
}


} // ns
} // ns
