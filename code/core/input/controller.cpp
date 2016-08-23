#include <core/input/controller.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/context_data/input_pool.hpp>
#include <utilities/logging.hpp>
#include <SDL2/SDL_scancode.h>
#include <assert.h>


namespace Core {


struct Controller::Impl
{
  uint32_t controller_number;
  
  std::shared_ptr<const Context_detail::Data> context_data;
};


Controller::Controller(const Core::Context &ctx, const uint32_t controller_id)
: m_impl(new Impl{controller_id, ctx.get_context_data()})
{
  assert(m_impl->context_data);
}


Controller::~Controller()
{
}


Controller::Controller(const Controller &other)
: m_impl(new Impl(*other.m_impl))
{
}

  
Controller&
Controller::operator=(const Controller &other)
{
  *m_impl = *other.m_impl;

  return *this;
}


Axis
Controller::get_axis(const uint8_t axis) const
{
  constexpr uint8_t max_axes = 2;

  assert(m_impl && m_impl->context_data);
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input && input->controller_count > m_impl->controller_number && axis < max_axes)
  {
    const Axis return_gp_axis = input->controllers[m_impl->controller_number].axis[axis];
    
    /*
      If we are gamepad 0
      Then check the mouse and keyboard if gp returned nothing to use.
    */
    if((m_impl->controller_number == 0) &&
      return_gp_axis.x == 0.f &&
      return_gp_axis.y == 0.f)
    {
      // We can think of mouse as axis 1
      // ie head rotation.
      if(axis == 1)
      {
        constexpr float scale_factor = 35.f; // TODO: This should be customisable.
      
        const Axis mouse_axis = input->mice[0].delta;
        const Axis normialized_axis = Axis{mouse_axis.x / scale_factor, mouse_axis.y / scale_factor};
        
        if(math::abs(normialized_axis.x) > 0.2f || math::abs(normialized_axis.y) > 0.2f)
        {
          return normialized_axis;
        }
      }
      
      // We can think of wasd as movement.
      // ie axis movement.
      else if(axis == 0)
      {
        float x_axis = 0.f;
        float y_axis = 0.f;
        
        using But_state = Core::Button_state;
        
        const But_state w_key = input->keyboard[SDL_SCANCODE_W];
        const But_state a_key = input->keyboard[SDL_SCANCODE_A];
        const But_state s_key = input->keyboard[SDL_SCANCODE_S];
        const But_state d_key = input->keyboard[SDL_SCANCODE_D];
        
        if(w_key == But_state::down || w_key == Button_state::down_on_frame)
        {
          y_axis += 1.f;
        }
        if(a_key == But_state::down || a_key == Button_state::down_on_frame)
        {
          x_axis -= 1.f;
        }
        if(s_key == But_state::down || s_key == Button_state::down_on_frame)
        {
          y_axis -= 1.f;
        }
        if(d_key == But_state::down || d_key == Button_state::down_on_frame)
        {
          x_axis += 1.f;
        }
        
        // Normalize them so similar to analog stick.
        if(math::abs(x_axis) == 1.f && math::abs(y_axis) == 1.f)
        {
          x_axis = math::sign(x_axis) * math::root_two();
          y_axis = math::sign(y_axis) * math::root_two();
        }
        
        return Axis{x_axis, y_axis};
      }
    }
    
    return return_gp_axis;
  }

  return Axis();
}


float
Controller::get_trigger(const uint8_t trigger) const
{
  assert(m_impl && m_impl->context_data);
  
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;
  assert(input);
  
  constexpr uint8_t max_triggers = 2;
  
  float return_value = 0.f;

  if(input && input->controller_count > m_impl->controller_number && trigger < max_triggers)
  {
    return_value = input->controllers[m_impl->controller_number].triggers[trigger];
    
    // If primary gamepad also check left mouse button.
    if(m_impl->controller_number == 0 && return_value == 0.f)
    {
      const Core::Button_state *buttons = &input->mice[0].buttons[0];
    
      if(buttons[0] == Button_state::down_on_frame || buttons[0] == Button_state::down)
      {
        return_value = 1.f;
      }
    }
  }

  return return_value;
}


namespace {


/*
  Checks if a bitfield of buttons corrisponds to any
  of the states provided.
*/
inline bool
is_button(Context_data::Input_pool *input,
          const uint32_t buttons,
          const uint32_t state_a,
          const uint32_t state_b,
          const uint32_t controller_id)
{
  assert(input);
  uint32_t button_state = false;

  if(input && input->controller_count > controller_id)
  {
    if(buttons & Core::Gamepad_button::ENUM::button_a)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_a;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      { 
        const uint32_t ms_state = (uint32_t)input->mice[0].buttons[0];
        button_state |= ((ms_state == state_a) || (ms_state == state_b));;
      }
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_b)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_b;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      { 
        const uint32_t kb_state_1 = (uint32_t)input->keyboard[SDL_SCANCODE_SPACE];
        button_state |= ((kb_state_1 == state_a) || (kb_state_1 == state_b));
      }
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_x)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_x;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_y)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_y;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_start)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_start;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      {
        const uint32_t kb_state_1 = (uint32_t)input->keyboard[SDL_SCANCODE_RETURN];
        button_state |= ((kb_state_1 == state_a) || (kb_state_1 == state_b));
        
        const uint32_t kb_state_2 = (uint32_t)input->keyboard[SDL_SCANCODE_RETURN2];
        button_state |= ((kb_state_2 == state_a) || (kb_state_2 == state_b));
      }
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_back)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_back;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      {
        const uint32_t kb_state = (uint32_t)input->keyboard[SDL_SCANCODE_ESCAPE];
        button_state |= ((kb_state == state_a) || (kb_state == state_b));
      }
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_left_shoulder)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_left_shoulder;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_right_shoulder)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_right_shoulder;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_left_stick)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_left_stick;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_right_stick)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_right_stick;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_dpad_up)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_dpad_up;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      {
        const uint32_t kb_state = (uint32_t)input->keyboard[SDL_SCANCODE_UP];
        button_state |= ((kb_state == state_a) || (kb_state == state_b));
      }
    }
    
    if(buttons & Core::Gamepad_button::ENUM::button_dpad_down)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_dpad_down;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      {
        const uint32_t kb_state = (uint32_t)input->keyboard[SDL_SCANCODE_DOWN];
        button_state |= ((kb_state == state_a) || (kb_state == state_b));
      }
    }

    if(buttons & Core::Gamepad_button::ENUM::button_dpad_left)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_dpad_left;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      {
        const uint32_t kb_state = (uint32_t)input->keyboard[SDL_SCANCODE_LEFT];
        button_state |= ((kb_state == state_a) || (kb_state == state_b));
      }
    }

    if(buttons & Core::Gamepad_button::ENUM::button_dpad_right)
    {
      const uint32_t gp_state = input->controllers[controller_id].controller_buttons.button_dpad_right;
      button_state |= ((gp_state == state_a) || (gp_state == state_b));
      
      if(controller_id == 0)
      {
        const uint32_t kb_state = (uint32_t)input->keyboard[SDL_SCANCODE_RIGHT];
        button_state |= ((kb_state == state_a) || (kb_state == state_b));
      }
    }
  }

  return !!button_state;
}


} // ns


bool
Controller::is_button_down(const Gamepad_button::ENUM buttons) const
{
  return is_button(m_impl->context_data->input_pool,
                   buttons,
                   (uint32_t)Core::Button_state::down,
                   (uint32_t)Core::Button_state::down_on_frame,
                   m_impl->controller_number);
}


bool
Controller::is_button_down_on_frame(const Gamepad_button::ENUM buttons) const
{
  return is_button(m_impl->context_data->input_pool,
                   buttons,
                   (uint32_t)Core::Button_state::down_on_frame,
                   (uint32_t)Core::Button_state::down_on_frame,
                   m_impl->controller_number);
}
 
 
bool
Controller::is_button_up(const Gamepad_button::ENUM buttons) const
{
  return is_button(m_impl->context_data->input_pool,
                   buttons,
                   (uint32_t)Core::Button_state::up,
                   (uint32_t)Core::Button_state::up_on_frame,
                   m_impl->controller_number);
}


bool
Controller::is_button_up_on_frame(const Gamepad_button::ENUM buttons) const
{
  return is_button(m_impl->context_data->input_pool,
                   buttons,
                   (uint32_t)Core::Button_state::up_on_frame,
                   (uint32_t)Core::Button_state::up_on_frame,
                   m_impl->controller_number);
}


} // ns