#ifndef INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E
#define INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E


#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>
#include <stddef.h>


namespace Core_data {


struct Controller
{
  ::Core::Input::Axis axis[2];
  ::Core::Input::Button_state buttons[16];
};



struct Input_pool
{
  Controller controllers[4];
}; // struct


void
input_data_init(Input_pool *data);


void
input_data_update_controller(Input_pool *pool,
                             const size_t controller,
                             const Controller *controller_data);


} // ns


#endif // inc guard