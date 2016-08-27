#include "sdl_input.hpp"
#include <SDL2/SDL.h>
#include <math/general/general.hpp>
#include <utilities/logging.hpp>
#include <utilities/optimizations.hpp>


namespace Sdl {


namespace
{
  inline Core::Button_state
  button_down(const Core::Button_state *curr_state)
  {
    if(*curr_state == Core::Button_state::down)          { return Core::Button_state::down; }
    if(*curr_state == Core::Button_state::down_on_frame) { return Core::Button_state::down; }
    
    return Core::Button_state::down_on_frame;
  };
  
  inline Core::Button_state
  button_up(const Core::Button_state *curr_state)
  {
    if(*curr_state == Core::Button_state::up)          { return Core::Button_state::up; }
    if(*curr_state == Core::Button_state::up_on_frame) { return Core::Button_state::up; }
    
    return Core::Button_state::up_on_frame;
  };
}


void
update_gamepad_controller(Context_data::Game_controller *controller, const uint32_t controller_id)
{
  SDL_GameController *sdl_controller = SDL_GameControllerOpen(controller_id);

  constexpr float sdl_axis_range       = 32767.f; // SDL's max axis range.
  constexpr float sdl_axis_granularity = 0.2f;    // Creates a dead zone in the center of the stick to eliminate drift.

  // Left Axis
  {
    //const Sint16 left_x = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTX);
    //const Sint16 left_y = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_LEFTY);
    

    //controller->axis[0].x = math::nearest_floor(static_cast<float>(left_x) / sdl_axis_range, sdl_axis_granularity);
    //controller->axis[0].y = math::nearest_floor(static_cast<float>(left_y) / sdl_axis_range, sdl_axis_granularity);
  }
  
  // Right Axis
  {
    //const Sint16 right_x = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTX);
    //const Sint16 right_y = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_RIGHTY);
    
    //controller->axis[1].x = math::nearest_floor(static_cast<float>(right_x) / sdl_axis_range, sdl_axis_granularity);
    //controller->axis[1].y = math::nearest_floor(static_cast<float>(right_y) / sdl_axis_range, sdl_axis_granularity);
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
//    Core::Button_state *button_state_0 = &controller->buttons[Core::Button::button_0];
//    const Uint8 button_a = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_A);
//    *button_state_0 = button_a ? button_down(button_state_0) : button_up(button_state_0);
//
//    Core::Button_state *button_state_1 = &controller->buttons[Core::Button::button_1];
//    const Uint8 button_b = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_B);
//    *button_state_1 = button_b ? button_down(button_state_1) : button_up(button_state_1);
//
//    Core::Button_state *button_state_2 = &controller->buttons[Core::Button::button_2];
//    const Uint8 button_x = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_X);
//    *button_state_2 = button_x ? button_down(button_state_2) : button_up(button_state_2);
//
//    Core::Button_state *button_state_3 = &controller->buttons[Core::Button::button_3];
//    const Uint8 button_y = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_Y);
//    *button_state_3 = button_y ? button_down(button_state_3) : button_up(button_state_3);
//
//    Core::Button_state *button_state_4 = &controller->buttons[Core::Button::button_4];
//    const Uint8 button_start = SDL_GameControllerGetButton(sdl_controller, SDL_CONTROLLER_BUTTON_START);
//    *button_state_4 = button_start ? button_down(button_state_4) : button_up(button_state_4);
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

      if(key_state[SDL_SCANCODE_A]) { axis_01_x = -1; }
      if(key_state[SDL_SCANCODE_D]) { axis_01_x = +1; }
      if(key_state[SDL_SCANCODE_W]) { axis_01_y = +1; }
      if(key_state[SDL_SCANCODE_S]) { axis_01_y = -1; }
      
      controller->axis[0].x = axis_01_x;
      controller->axis[0].y = axis_01_y;
    }
    
    // Axis 02
    {
      float axis_02_x = 0;
      float axis_02_y = 0;

      if(key_state[SDL_SCANCODE_LEFT])  { axis_02_x = -1; }
      if(key_state[SDL_SCANCODE_RIGHT]) { axis_02_x = +1; }
      if(key_state[SDL_SCANCODE_UP])    { axis_02_y = +1; }
      if(key_state[SDL_SCANCODE_DOWN])  { axis_02_y = -1; }
      
      controller->axis[1].x = axis_02_x;
      controller->axis[1].y = axis_02_y;
    }

    // Keys
    {
      // Mouse keys
      {
//        Core::Button_state *button_state_0 = &controller->buttons[Core::Button::button_0];
//        *button_state_0 = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) ? button_down(button_state_0) : button_up(button_state_0);
//
//        Core::Button_state *button_state_1 = &controller->buttons[Core::Button::button_1];
//        *button_state_1 = mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT) ? button_down(button_state_1) : button_up(button_state_1);
//      }
//      
//      // Keyboard
//      {
//        Core::Button_state *button_state_4 = &controller->buttons[Core::Button::button_2];
//        *button_state_4 = key_state[SDL_SCANCODE_LSHIFT] ? button_down(button_state_4) : button_up(button_state_4);
//
//        Core::Button_state *button_state_5 = &controller->buttons[Core::Button::button_3];
//        *button_state_5 = key_state[SDL_SCANCODE_SPACE] ? button_down(button_state_5) : button_up(button_state_5);
//        
//        Core::Button_state *button_state_6 = &controller->buttons[Core::Button::button_4];
//        *button_state_6 = key_state[SDL_SCANCODE_RETURN] ? button_down(button_state_6) : button_up(button_state_6);
      }
    }
  }
}


void
capture_mouse(const bool set)
{
  const SDL_bool set_mouse_hold = set ? SDL_TRUE : SDL_FALSE;
  
  int success = SDL_SetRelativeMouseMode(set_mouse_hold);
  assert(success == 0);
  
  #ifndef NDEBUG
  if(set)
  {
    assert(SDL_GetRelativeMouseMode());
    
    #ifdef __APPLE__
    // https://forums.libsdl.org/viewtopic.php?p=51127&sid=570a47d0f562cc0b1d4c91b7712c663f
    //SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
    LOG_WARNING("This is broken on Apple right now: Mouse motion events no longer get fired!");
    #endif
  }
  #endif
}


bool
is_mouse_captured()
{
  const SDL_bool is_mouse_held = SDL_GetRelativeMouseMode();
  return is_mouse_held == SDL_TRUE;
}


namespace {

inline void
on_button_event(Context_data::Game_controller *controller,
                const Uint8 key,
                const uint32_t button_event)
{
  switch(key)
  {
    case(SDL_CONTROLLER_BUTTON_A):
      controller->controller_buttons.button_a = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_B):
      controller->controller_buttons.button_b = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_X):
      controller->controller_buttons.button_x = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_Y):
      controller->controller_buttons.button_y = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_BACK):
      controller->controller_buttons.button_back = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_START):
      controller->controller_buttons.button_start = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_LEFTSTICK):
      controller->controller_buttons.button_left_stick = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_RIGHTSTICK):
      controller->controller_buttons.button_right_stick = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_LEFTSHOULDER):
      controller->controller_buttons.button_left_shoulder = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER):
      controller->controller_buttons.button_right_shoulder = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_DPAD_UP):
      controller->controller_buttons.button_dpad_up = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_DPAD_DOWN):
      controller->controller_buttons.button_dpad_down = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_DPAD_LEFT):
      controller->controller_buttons.button_dpad_left = button_event;
      return;
      
    case(SDL_CONTROLLER_BUTTON_DPAD_RIGHT):
      controller->controller_buttons.button_dpad_right = button_event;
      return;
      
    default:
      return;
  };
  UNREACHABLE;
}


inline void
on_mouse_button_event(Context_data::Mouse *mouse,
                      const Uint8 key,
                      const Core::Button_state button_event)
{
  switch(key)
  {
    case(SDL_BUTTON_LEFT):
      mouse->buttons[0] = button_event;
      return;
      
    case(SDL_BUTTON_MIDDLE):
      mouse->buttons[1] = button_event;
      return;
      
    case(SDL_BUTTON_RIGHT):
      mouse->buttons[2] = button_event;
      return;
      
    case(SDL_BUTTON_X1):
      mouse->buttons[3] = button_event;
      return;
      
    case(SDL_BUTTON_X2):
      mouse->buttons[4] = button_event;
      return;
  };
  UNREACHABLE;
}

} // anon ns


void
process_input_messages(const SDL_Event *evt,
                       Context_data::Input_pool *input_data)
{
  if(!input_data)
  {
    return;
  }

  // Reset mouse delta
  for(uint32_t i = 0; i < input_data->mice_count; ++i)
  {
    input_data->mice[i].delta = Core::Axis{0.f,0.f};
  }
  
  constexpr float sdl_axis_range       = 32767.f; // SDL's max axis range.
  constexpr float sdl_axis_granularity = 0.2f;    // Creates a dead zone in the center of the stick to eliminate drift.
  
  switch(evt->type)
  {
    /*
      Mouse Events
    */

    case(SDL_MOUSEMOTION):
    {
      const uint32_t mouse_id = 0;
      assert(mouse_id < input_data->mice_count);
      
      input_data->mice[mouse_id].delta.x = math::to_float(evt->motion.xrel);
      input_data->mice[mouse_id].delta.y = math::to_float(evt->motion.yrel);

      input_data->mice[mouse_id].position.x = math::to_float(evt->motion.x);
      input_data->mice[mouse_id].position.y = math::to_float(evt->motion.y);
      
      break;
    } // SDL_MOUSEMOTION
    
    case(SDL_MOUSEWHEEL):
    {
      break;
    } // SDL_MOUSEWHEEL
    
    case(SDL_MOUSEBUTTONDOWN):
    {
      const uint32_t mouse_id = 0;
      assert(mouse_id < input_data->mice_count);
    
      if(mouse_id < input_data->mice_count)
      {
        Context_data::Mouse *mouse = &input_data->mice[mouse_id];
        on_mouse_button_event(mouse, evt->button.button, Core::Button_state::down_on_frame);
      }
      
      break;
    } // SDL_MOUSEBUTTONDOWN
    
    case(SDL_MOUSEBUTTONUP):
    {
      const uint32_t mouse_id = 0;
      assert(mouse_id < input_data->mice_count);
    
      if(mouse_id < input_data->mice_count)
      {
        Context_data::Mouse *mouse = &input_data->mice[mouse_id];
        on_mouse_button_event(mouse, evt->button.button, Core::Button_state::up_on_frame);
      }
      
      break;
    } // SDL_MOUSEBUTTONUP
    
    
    /*
      Keyboard Events
    */

    case(SDL_KEYDOWN):
    {
      input_data->keyboard[evt->key.keysym.scancode] = Core::Button_state::down_on_frame;
      break;
    } // SDL_KEYDOWN
    
    case(SDL_KEYUP):
    {
      input_data->keyboard[evt->key.keysym.scancode] = Core::Button_state::up_on_frame;
      break;
    } // SDL_KEYUP
    
    
    /*
      Gamepad events.
    */
  
    case(SDL_JOYBUTTONDOWN):
    case(SDL_CONTROLLERBUTTONDOWN):
    {
      const uint32_t button_event = (uint32_t)Core::Button_state::down_on_frame;
      Context_data::Game_controller *controller = &input_data->controllers[evt->cbutton.which];
      
      on_button_event(controller, evt->cbutton.button, button_event);
      break;
    } // SDL_CONTROLLERBUTTONDOWN

    case(SDL_JOYBUTTONUP):
    case(SDL_CONTROLLERBUTTONUP):
    {
      const uint32_t button_event = (uint32_t)Core::Button_state::up_on_frame;
      Context_data::Game_controller *controller = &input_data->controllers[evt->cbutton.which];
      
      on_button_event(controller, evt->cbutton.button, button_event);
      break;
    } // SDL_CONTROLLERBUTTONUP

    case(SDL_JOYAXISMOTION):
    case(SDL_CONTROLLERAXISMOTION):
    {
      const uint32_t controller_id = evt->caxis.which;
      assert(controller_id < input_data->controller_count);

      if(controller_id < input_data->controller_count)
      {
        const float norm  = math::to_float(evt->caxis.value) / sdl_axis_range;
        const float value = math::nearest_floor(norm, sdl_axis_granularity);

        switch(evt->caxis.axis)
        {
          case(SDL_CONTROLLER_AXIS_LEFTX):
            input_data->controllers[controller_id].axis[0].x = value;
            break;

          case(SDL_CONTROLLER_AXIS_LEFTY):
            input_data->controllers[controller_id].axis[0].y = value;
            break;

          case(SDL_CONTROLLER_AXIS_RIGHTX):
            input_data->controllers[controller_id].axis[1].x = value;
            break;

          case(SDL_CONTROLLER_AXIS_RIGHTY):
            input_data->controllers[controller_id].axis[1].y = value;
            break;
            
          case(SDL_CONTROLLER_AXIS_TRIGGERLEFT):
            input_data->controllers[controller_id].triggers[0] = value;
            break;
            
          case(SDL_CONTROLLER_AXIS_TRIGGERRIGHT):
            input_data->controllers[controller_id].triggers[1] = value == -1 ? 0 : math::abs(value); // TODO: No idea why!
            break;
        }
      }
      
      break;
    } // SDL_CONTROLLERAXISMOTION
    
    
  } // Switch
}

} // ns