#include <core/input/keyboard.hpp>
#include <core/context/context.hpp>
#include <core/context/detail/context_detail.hpp>
#include <data/context_data/input_pool.hpp>


namespace Core {


struct Keyboard::Impl
{
  uint32_t keybaord_id;
  std::shared_ptr<const Context_detail::Data> context_data;
};


Keyboard::Keyboard()
: m_impl(nullptr)
{
}


Keyboard::Keyboard(Core::Context &ctx)
: m_impl(new Impl)
{
  m_impl->keybaord_id = 0;
  m_impl->context_data = ctx.get_context_data();
};


bool
Keyboard::is_valid() const
{
  return !!m_impl->context_data;
}


Keyboard::operator bool() const
{
  return is_valid();
}


} // ns