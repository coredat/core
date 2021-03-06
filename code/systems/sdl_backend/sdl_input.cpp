#include "sdl_input.hpp"
#include <SDL2/SDL.h>
#include <math/general/general.hpp>
#include <utilities/logging.hpp>
#include <utilities/optimizations.hpp>


namespace {


constexpr float sdl_axis_range       = 32767.f; // SDL's max axis range.
constexpr float sdl_axis_granularity = 0.2f;    // Creates a dead zone in the center of the stick to eliminate drift.
constexpr float sdl_touchpad_upscale = 5000.f;  // Upscale touchpad input events so they can be used as alternative mouse delta's.


} // anon ns


namespace Sdl {


namespace {

  Text_input_stream_fn input_stream_callback = nullptr;
  
} // anon ns


void
get_text_input_stream(const Text_input_stream_fn &text_input_stream)
{
  input_stream_callback = text_input_stream;
}


void
update_gamepad_controller(Context_data::Game_controller *controller, const uint32_t controller_id)
{
  SDL_GameController *sdl_controller = SDL_GameControllerOpen(controller_id);

  // Triggers
  {
    const Sint16 left_trig  = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    const Sint16 right_trig = SDL_GameControllerGetAxis(sdl_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    
    controller->triggers[0] = math::nearest_floor(static_cast<float>(left_trig)  / sdl_axis_range, sdl_axis_granularity);
    controller->triggers[1] = math::nearest_floor(static_cast<float>(right_trig) / sdl_axis_range, sdl_axis_granularity);
  }
  
  SDL_GameControllerClose(sdl_controller);
}


void
update_keyboard_controller(Context_data::Game_controller *controller)
{
  const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
  
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
  }
}


void
capture_mouse(const bool set)
{
  const int success = SDL_SetRelativeMouseMode(set ? SDL_TRUE : SDL_FALSE);
  assert(success == 0);
  
  #ifndef NDEBUG
  if(set)
  {
    assert(SDL_GetRelativeMouseMode());
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


inline void
controller_used(bool *in_use)
{
  if(!(*in_use))
  {
    *in_use = true;
  }
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
    
    case(SDL_FINGERMOTION):
    {
      const uint32_t mouse_id = 0;
      assert(mouse_id < input_data->mice_count);

      input_data->mice[mouse_id].delta.x = evt->tfinger.dx * sdl_touchpad_upscale;
      input_data->mice[mouse_id].delta.y = evt->tfinger.dy * sdl_touchpad_upscale;

      break;
    } // SDL_FINGERMOTION
    
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
      
      if(input_stream_callback)
      {
        switch(evt->key.keysym.scancode)
        {
          case(SDL_SCANCODE_BACKSPACE):
          {
            constexpr char backspace[] = "\b";
            input_stream_callback((uint8_t*)backspace, strlen(backspace));
            break;
          }
          case(SDL_SCANCODE_RETURN):
          case(SDL_SCANCODE_RETURN2):
          {
            constexpr char newline[] = "\n";
            input_stream_callback((uint8_t*)newline, strlen(newline));
            break;
          }
          
          default: ; // Just to shutup warning.
        }
      }
      
      break;
    } // SDL_KEYUP
    
    /*
      Text streaming
    */
    case(SDL_TEXTINPUT):
    {
      if(input_stream_callback)
      {
        input_stream_callback((uint8_t*)evt->text.text, strlen(evt->text.text));
      }
      
      break;
    }
    
    /*
      Gamepad events.
    */
  
    //case(SDL_JOYBUTTONDOWN):
    case(SDL_CONTROLLERBUTTONDOWN):
    {
      controller_used(&input_data->controllers_touched);
    
      const uint32_t button_event = (uint32_t)Core::Button_state::down_on_frame;
      Context_data::Game_controller *controller = &input_data->controllers[evt->cbutton.which];
      
      on_button_event(controller, evt->cbutton.button, button_event);
      break;
    } // SDL_CONTROLLERBUTTONDOWN

    //case(SDL_JOYBUTTONUP):
    case(SDL_CONTROLLERBUTTONUP):
    {
      controller_used(&input_data->controllers_touched);
    
      const uint32_t button_event = (uint32_t)Core::Button_state::up_on_frame;
      Context_data::Game_controller *controller = &input_data->controllers[evt->cbutton.which];
      
      on_button_event(controller, evt->cbutton.button, button_event);
      break;
    } // SDL_CONTROLLERBUTTONUP

    //case(SDL_JOYAXISMOTION):
    case(SDL_CONTROLLERAXISMOTION):
    {    
      const uint32_t controller_id = evt->caxis.which;
      assert(controller_id < input_data->controller_count);

      if(controller_id < input_data->controller_count)
      {
        const float norm  = math::to_float(evt->caxis.value) / sdl_axis_range;
        const float value = math::nearest_floor(norm, sdl_axis_granularity);

        if(math::abs(value) > sdl_axis_granularity)
        {
          controller_used(&input_data->controllers_touched);
        }

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
            // This trigger can sometimes come back as neg values.
            input_data->controllers[controller_id].triggers[1] = value == -1 ? 0 : math::abs(value);
            break;
        }
      }
      
      break;
    } // SDL_CONTROLLERAXISMOTION
    
    
  } // Switch
}

} // ns