#include "sdl_input.hpp"
#include <SDL2/SDL.h>


namespace Sdl {


void
update_keyboard_controller(Core_data::Game_controller *controller)
{
  const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
  const Uint32 mouse_state = SDL_GetMouseState(nullptr, nullptr);
  
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
      controller->buttons[Core::Input::Button::button_0] = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) ? Core::Input::Button_state::down : Core::Input::Button_state::up;
      controller->buttons[Core::Input::Button::button_1] = mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT) ? Core::Input::Button_state::down : Core::Input::Button_state::up;
      controller->buttons[Core::Input::Button::button_2] = (key_state[SDL_SCANCODE_LSHIFT] ? Core::Input::Button_state::down : Core::Input::Button_state::up);
      controller->buttons[Core::Input::Button::button_3] = (key_state[SDL_SCANCODE_SPACE] ? Core::Input::Button_state::down : Core::Input::Button_state::up);
      controller->buttons[Core::Input::Button::button_4] = (key_state[SDL_SCANCODE_RETURN] ? Core::Input::Button_state::down : Core::Input::Button_state::up);
    }
  }
}


void
capture_mouse(const bool set)
{
  const SDL_bool set_mouse_hold = set ? SDL_TRUE : SDL_FALSE;
  SDL_SetRelativeMouseMode(set_mouse_hold);
}


bool
is_mouse_captured()
{
  const SDL_bool is_mouse_held = SDL_GetRelativeMouseMode();
  return is_mouse_held == SDL_TRUE;
}

} // ns