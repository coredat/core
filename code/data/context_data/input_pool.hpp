#ifndef INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E
#define INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E


#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>
#include <stdint.h>


#define NUMBER_OF_CONTROLLERS 4


namespace Core_data {


struct Game_controller
{
  ::Core::Input::Axis axis[2];
  float triggers[2];
  ::Core::Input::Button_state buttons[16];
}; // struct


struct Input_pool
{
  Game_controller controllers[NUMBER_OF_CONTROLLERS];
  
  const uint32_t size = NUMBER_OF_CONTROLLERS;
}; // struct


/*!
  Initialises the data pool
  \param pool is the data pool you wish to initialise.
*/
void
input_data_init(Input_pool *pool);


/*!
  Update a controller's data.
  \param pool, the data pool.
  \param controller the controller number.
  \param controller_data is the new data.
*/
void
input_data_update_controller(Input_pool *pool,
                             const uint32_t controller,
                             const Game_controller *controller_data);


/*!
  Get the current state of a controller.
  \param pool is the input data.
  \param controller index.
*/
Game_controller*
input_data_get_controller(Input_pool *pool, const uint32_t controller);


} // ns


#endif // inc guard