#include "input.hpp"


namespace Core {
namespace Input {


Controller
get_controller(const Player::ENUM player)
{
  auto core_data  = Data::get_core_data();
  auto input_data = Data::get_input_data(core_data);

  Controller controller;
  Detail::set_members(&controller, input_data, (size_t)player);

  return controller;
}


} // ns
} // ns
