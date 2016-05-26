#include <core/input/controller.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/context_data/input_pool.hpp>
#include <utilities/logging.hpp>
#include <assert.h>
#include <cstring>


namespace Core {
namespace Input {


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


Controller::Controller(Controller &&other)
: m_impl(new Impl(*other.m_impl))
{
}

  
Controller&
Controller::operator=(const Controller &other)
{
  *m_impl = *other.m_impl;

  return *this;
}


Controller&
Controller::operator=(Controller &&other)
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

  if(input && input->size > m_impl->controller_number && axis < max_axes)
  {
    return input->controllers[m_impl->controller_number].axis[axis];
  }

  return Axis();
}


float
Controller::get_trigger(const uint8_t trigger) const
{
  constexpr uint8_t max_triggers = 2;

  assert(m_impl && m_impl->context_data);
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input && input->size > m_impl->controller_number && trigger < max_triggers)
  {
    return input->controllers[m_impl->controller_number].triggers[trigger];
  }

  return 0.f;
}


namespace
{
  inline Core::Input::Button_state
  get_state(const Context_data::Input_pool *data, const uint32_t controller, const uint32_t button_id)
  {
    return data->controllers[controller].buttons[button_id];
  }
}


bool
Controller::is_button_down(const Button::ENUM button) const
{
  assert(m_impl && m_impl->context_data);
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input && input->size > m_impl->controller_number)
  {
    const Core::Input::Button_state curr_state = get_state(input, m_impl->controller_number, button);
    return curr_state == Core::Input::Button_state::down || curr_state == Core::Input::Button_state::down_on_frame;
  }

  return false;
}


bool
Controller::is_button_down_on_frame(const Button::ENUM button) const
{
  assert(m_impl && m_impl->context_data);
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input && input->size > m_impl->controller_number)
  {
    const Core::Input::Button_state curr_state = get_state(input, m_impl->controller_number, button);
    return curr_state == Core::Input::Button_state::down_on_frame;
  }

  return false;
}
 
 
bool
Controller::is_button_up(const Button::ENUM button) const
{
  assert(m_impl && m_impl->context_data);
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input && input->size > m_impl->controller_number)
  {
    const Core::Input::Button_state curr_state = get_state(input, m_impl->controller_number, button);
    return curr_state== Core::Input::Button_state::up || curr_state == Core::Input::Button_state::up_on_frame;
  }

  return false;
}


bool
Controller::is_button_up_on_frame(const Button::ENUM button) const
{
  assert(m_impl && m_impl->context_data);
  const Context_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input && input->size > m_impl->controller_number)
  {
    const Core::Input::Button_state curr_state = get_state(input, m_impl->controller_number, button);
    return curr_state == Core::Input::Button_state::up_on_frame;
  }

  return false;
}


} // ns
} // ns