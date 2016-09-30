#include <core/input/mouse.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/context_data/input_pool.hpp>


namespace Core {


struct Mouse::Impl
{
  std::shared_ptr<const Context_detail::Data> context_data;
};


Mouse::Mouse()
: m_impl(nullptr)
{
}


Mouse::Mouse(Core::Context &ctx)
: m_impl(new Impl)
{
  m_impl->context_data = ctx.get_context_data();
}


Mouse::~Mouse()
{
}


Axis
Mouse::get_delta_position() const
{
  if(is_valid())
  {
    return m_impl->context_data->input_pool->mice[0].delta;
  }
  
  return Axis{0.f, 0.f};
}


Axis
Mouse::get_position() const
{
  if(is_valid())
  {
    return m_impl->context_data->input_pool->mice[0].position;
  }
  
  return Axis{0.f, 0.f};
}


namespace {

inline bool
is_button(Context_data::Input_pool *input,
          const uint32_t buttons,
          const Core::Button_state state_a,
          const Core::Button_state state_b,
          const uint32_t mouse_id)
{
  assert(input);
  bool button_state = false;

  if(input && input->mice_count > mouse_id)
  {
    if(buttons & Core::Mouse_button::ENUM::left)
    {
      const Core::Button_state ms_state = input->mice[mouse_id].buttons[Core::Mouse_button::ENUM::left];
      button_state |= ((ms_state == state_a) || (ms_state == state_b));
    }
  }
  
  return button_state;
}

} // anon ns


bool
Mouse::is_button_down(const uint32_t mouse_buttons) const
{
  if(is_valid())
  {
    return is_button(m_impl->context_data->input_pool,
                     mouse_buttons,
                     Core::Button_state::down,
                     Core::Button_state::down_on_frame,
                     0);
  }
  
  return false;
}


bool
Mouse::is_button_down_on_frame(const uint32_t mouse_buttons) const
{
  if(is_valid())
  {
    return is_button(m_impl->context_data->input_pool,
                     mouse_buttons,
                     Core::Button_state::down_on_frame,
                     Core::Button_state::down_on_frame,
                     0);
  }
  
  return false;
}


bool
Mouse::is_button_up(const uint32_t mouse_buttons) const
{
  if(is_valid())
  {
    return is_button(m_impl->context_data->input_pool,
                     mouse_buttons,
                     Core::Button_state::up,
                     Core::Button_state::up_on_frame,
                     0);
  }
  
  return false;
}


bool
Mouse::is_button_up_on_frame(const uint32_t mouse_buttons) const
{
  if(is_valid())
  {
    return is_button(m_impl->context_data->input_pool,
                     mouse_buttons,
                     Core::Button_state::up_on_frame,
                     Core::Button_state::up_on_frame,
                     0);
  }
  
  return false;
}


bool
Mouse::is_valid() const
{
  return !!m_impl;
}

Mouse::operator
bool() const
{
  return is_valid();
}


} // ns