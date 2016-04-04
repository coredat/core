#include <core/input/controller.hpp>
#include <core/context/context.hpp>
#include <data/core_data/input_pool.hpp>
#include <data/core_data/core_data.hpp>
#include <utilities/logging.hpp>
#include <assert.h>
#include <cstring>


namespace Core {
namespace Input {


struct Controller::Impl
{
  Core_data::Input_pool *input_data = nullptr;
  uint32_t controller_number = 0;
};


Controller::Controller(const Core::Context &ctx, const uint32_t controller_id)
: m_impl(new Impl{nullptr, controller_id})
{
  m_impl->input_data = static_cast<Core_data::Core*>(ctx.get_context_data())->input_pool;
  
  LOG_TODO("Better checks required in this class")
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
  return m_impl->input_data->controllers[m_impl->controller_number].axis[axis];
}


bool
Controller::is_button_down(const Button::ENUM button) const
{
  return m_impl->input_data->controllers[m_impl->controller_number].buttons[(int)button] == Core::Input::Button_state::down;
}


} // ns
} // ns