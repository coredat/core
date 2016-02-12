#ifndef INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E
#define INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E


#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>
#include <stddef.h>


#define NUMBER_OF_CONTROLLERS 4


namespace Core_data {


struct Game_controller
{
  ::Core::Input::Axis axis[2];
  ::Core::Input::Button_state buttons[16];
};



struct Input_pool
{
  Game_controller controllers[NUMBER_OF_CONTROLLERS];
  
  const size_t size = NUMBER_OF_CONTROLLERS;
}; // struct


void
input_data_init(Input_pool *data);


void
input_data_update_controller(Input_pool *pool,
                             const size_t controller,
                             const Game_controller *controller_data);
  
Game_controller*
input_data_get_controller(Input_pool *pool, const size_t controller);


} // ns


#endif // inc guard