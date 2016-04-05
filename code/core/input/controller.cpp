#include <core/input/controller.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/core_data/input_pool.hpp>
#include <data/core_data/core_data.hpp>
#include <utilities/logging.hpp>
#include <assert.h>
#include <cstring>


namespace Core {
namespace Input {


struct Controller::Impl
{
  uint32_t controller_number = 0;
  
  std::shared_ptr<const Context_detail::Context_data> context_data;
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
  assert(m_impl && m_impl->context_data);
  const Core_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input)
  {
    return m_impl->context_data->input_pool->controllers[0].axis[axis];
  }

  return Axis();
}


bool
Controller::is_button_down(const Button::ENUM button) const
{
  assert(m_impl && m_impl->context_data);
  const Core_data::Input_pool *input = m_impl->context_data->input_pool;

  if(input)
  {
    return input->controllers[0].buttons[(uint32_t)button] == Core::Input::Button_state::down;
  }

  return false;
}


} // ns
} // ns