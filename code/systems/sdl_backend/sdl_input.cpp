#include "sdl_input.hpp"
#include <SDL2/SDL.h>
#include <math/general/general.hpp>


namespace Sdl {


void
update_gamepad_controller(Context_data::Game_controller *controller, const uint32_t controller_id)
{
  SDL_GameController *sdl_controller = SDL_GameControllerOpen(controller_id);

  constexpr float sdl_axis_range = 32767.f;     // SDL's max axis range.
  constexpr float sdl_axis_granularity = 0.2f;  // Creates a dead zone in the center of the stick to eliminate drift.

  // Left Axis
  {
    const Sint16 left_x = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTX);
    const Sint16 left_y = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTY);
    
    controller->axis[0].x = math::nearest_floor(static_cast<float>(left_x) / sdl_axis_range, sdl_axis_granularity);
    controller->axis[0].y = math::nearest_floor(static_cast<float>(left_y) / sdl_axis_range, sdl_axis_granularity);
  }
  
  // Right Axis
  {
    const Sint16 right_x = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTX);
    const Sint16 right_y = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTY);
    
    controller->axis[1].x = math::nearest_floor(static_cast<float>(right_x) / sdl_axis_range, sdl_axis_granularity);
    controller->axis[1].y = math::nearest_floor(static_cast<float>(right_y) / sdl_axis_range, sdl_axis_granularity);
  }
  
  // Triggers
  {
    const Sint16 left_trig  = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    const Sint16 right_trig = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    
    controller->triggers[0] = math::nearest_floor(static_cast<float>(left_trig)  / sdl_axis_range, sdl_axis_granularity);
    controller->triggers[1] = math::nearest_floor(static_cast<float>(right_trig) / sdl_axis_range, sdl_axis_granularity);
  }
  
  // Buttons
  {
    const Uint8 button_a = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_A);
    controller->buttons[Core::Input::Button::button_0] = button_a ? Core::Input::Button_state::down : Core::Input::Button_state::up;

    const Uint8 button_b = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_B);
    controller->buttons[Core::Input::Button::button_1] = button_b ? Core::Input::Button_state::down : Core::Input::Button_state::up;

    const Uint8 button_x = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_X);
    controller->buttons[Core::Input::Button::button_2] = button_x ? Core::Input::Button_state::down : Core::Input::Button_state::up;

    const Uint8 button_y = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_Y);
    controller->buttons[Core::Input::Button::button_3] = button_y ? Core::Input::Button_state::down : Core::Input::Button_state::up;

    const Uint8 button_start = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_START);
    controller->buttons[Core::Input::Button::button_4] = button_start ? Core::Input::Button_state::down : Core::Input::Button_state::up;
  }
  
  SDL_GameControllerClose(sdl_controller);
}


void
update_keyboard_controller(Context_data::Game_controller *controller)
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