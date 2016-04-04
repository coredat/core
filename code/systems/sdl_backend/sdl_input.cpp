#include "sdl_input.hpp"
#include <SDL2/SDL.h>


namespace Sdl {


void
update_keyboard_controller(Core_data::Game_controller *controller)
{
  const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
  
  if(key_state)
  {
    // Axis 01
    {
      float axis_01_x = 0;
      float axis_01_y = 0;

      if(key_state[SDL_SCANCODE_A]) { axis_01_x -= 1; }
      if(key_state[SDL_SCANCODE_D]) { axis_01_x += 1; }
      if(key_state[SDL_SCANCODE_W]) { axis_01_y += 1; }
      if(key_state[SDL_SCANCODE_S]) { axis_01_y -= 1; }
      
      controller->axis[0].x = axis_01_x;
      controller->axis[0].y = axis_01_y;
    }
    
    // Axis 02
    {
      float axis_02_x = 0;
      float axis_02_y = 0;

      if(key_state[SDL_SCANCODE_LEFT])  { axis_02_x -= 1; }
      if(key_state[SDL_SCANCODE_RIGHT]) { axis_02_x += 1; }
      if(key_state[SDL_SCANCODE_UP])    { axis_02_y += 1; }
      if(key_state[SDL_SCANCODE_DOWN])  { axis_02_y -= 1; }
      
      controller->axis[1].x = axis_02_x;
      controller->axis[1].y = axis_02_y;
    }

    // Keys
    {
      // Not all keys are done here.
      // And ability to change default keys needs to be dealt with.
      
      controller->buttons[Core::Input::Button::button_2] = (key_state[SDL_SCANCODE_LSHIFT] ? Core::Input::Button_state::down : Core::Input::Button_state::up);
      controller->buttons[Core::Input::Button::button_3] = (key_state[SDL_SCANCODE_SPACE] ? Core::Input::Button_state::down : Core::Input::Button_state::up);
    }
  }
}


} // ns