#include <transformations/input/frame_update.hpp>
#include <core/input/buttons.hpp>
#include <assert.h>


namespace Input_utils {


void
update_input_state(Context_data::Input_pool *input_data)
{
  assert(input_data);

  /*
    Update gamepad events, they need to turn into their held versions.
  */
  for(uint32_t i = 0; i < input_data->controller_count; ++i)
  {
    Context_data::Gamepad_state *state = &input_data->controllers[i].controller_buttons;
    
    constexpr uint32_t up_evt   = static_cast<uint32_t>(Core::Button_state::up_on_frame);
    constexpr uint32_t up       = static_cast<uint32_t>(Core::Button_state::up);
    constexpr uint32_t down_evt = static_cast<uint32_t>(Core::Button_state::down_on_frame);
    constexpr uint32_t down     = static_cast<uint32_t>(Core::Button_state::down);
    
    /*
      Update the key states.
    */
    if(state->button_a == up_evt)                       { state->button_a = up;   }
    else if(state->button_a == down_evt)                { state->button_a = down; }

    if(state->button_b == up_evt)                       { state->button_b = up;   }
    else if(state->button_b == down_evt)                { state->button_b = down; }

    if(state->button_x == up_evt)                       { state->button_x = up;   }
    else if(state->button_x == down_evt)                { state->button_x = down; }

    if(state->button_y == up_evt)                       { state->button_y = up;   }
    else if(state->button_y == down_evt)                { state->button_y = down; }

    if(state->button_start == up_evt)                   { state->button_start = up;   }
    else if(state->button_start == down_evt)            { state->button_start = down; }

    if(state->button_back == up_evt)                    { state->button_back = up;    }
    else if(state->button_back == down_evt)             { state->button_back = down;  }

    if(state->button_left_shoulder == up_evt)           { state->button_left_shoulder = up;   }
    else if(state->button_left_shoulder == down_evt)    { state->button_left_shoulder = down; }

    if(state->button_right_shoulder == up_evt)          { state->button_right_shoulder = up;    }
    else if(state->button_right_shoulder == down_evt)   { state->button_right_shoulder = down;  }

    if(state->button_left_stick == up_evt)              { state->button_left_stick = up;    }
    else if(state->button_left_stick == down_evt)       { state->button_left_stick = down;  }

    if(state->button_right_stick == up_evt)             { state->button_right_stick = up;   }
    else if(state->button_right_stick == down_evt)      { state->button_right_stick = down; }

    if(state->button_dpad_up == up_evt)                 { state->button_dpad_up = up;   }
    else if(state->button_dpad_up == down_evt)          { state->button_dpad_up = down; }

    if(state->button_dpad_down == up_evt)               { state->button_dpad_down = up;   }
    else if(state->button_dpad_down == down_evt)        { state->button_dpad_down = down; }

    if(state->button_dpad_right == up_evt)              { state->button_dpad_right = up;    }
    else if(state->button_dpad_right == down_evt)       { state->button_dpad_right = down;  }

    if(state->button_dpad_left == up_evt)               { state->button_dpad_left = up;   }
    else if(state->button_dpad_left == down_evt)        { state->button_dpad_left = down; }
  }
  
  
  /*
    Update keyboard events
  */
  for(uint32_t i = 0; i < input_data->key_count; ++i)
  {
    if(input_data->keyboard[i] == Core::Button_state::up_on_frame)          { input_data->keyboard[i] = Core::Button_state::up;   }
    else if(input_data->keyboard[i] == Core::Button_state::down_on_frame)   { input_data->keyboard[i] = Core::Button_state::down; }
  }
}


} // ns