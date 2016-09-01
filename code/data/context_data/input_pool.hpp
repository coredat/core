#ifndef INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E
#define INPUT_POOL_INCLUDED_A7219D2F_B6D0_4669_947C_45C387DC209E


#include <core/input/axis.hpp>
#include <core/input/buttons.hpp>
#include <stdint.h>


// TODO: Kill these numbers
#define NUMBER_OF_CONTROLLERS 4
#define NUMBER_OF_MICE 1
#define NUMBER_OF_KEYS 512
#define NUMBER_OF_MOUSE_BUTTONS 5


namespace Context_data {


struct Gamepad_state
{
  uint32_t button_a : 2;
  uint32_t button_b : 2;
  uint32_t button_x : 2;
  uint32_t button_y : 2;
  uint32_t button_left_shoulder : 2;
  uint32_t button_right_shoulder : 2;
  uint32_t button_left_stick : 2;
  uint32_t button_right_stick : 2;
  uint32_t button_back : 2;
  uint32_t button_start : 2;
  uint32_t button_dpad_up : 2;
  uint32_t button_dpad_down : 2;
  uint32_t button_dpad_left : 2;
  uint32_t button_dpad_right : 2;
  uint32_t extra_01 : 2;
  uint32_t extra_02 : 2;
};


struct Game_controller
{
  Core::Axis axis[2];
  float triggers[2];
  Gamepad_state controller_buttons;
}; // struct


struct Mouse
{
  Core::Axis delta;
  Core::Axis position;
  Core::Button_state buttons[NUMBER_OF_MOUSE_BUTTONS];
};


struct Input_pool
{
  Game_controller controllers[NUMBER_OF_CONTROLLERS];
  const uint32_t controller_count = NUMBER_OF_CONTROLLERS;
  bool controllers_touched = false;

  Mouse mice[NUMBER_OF_MICE];
  const uint32_t mice_count = NUMBER_OF_MICE;
  
  Core::Button_state keyboard[NUMBER_OF_KEYS];
  const uint32_t key_count = NUMBER_OF_KEYS;
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