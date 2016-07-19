#include <core/physics/rigidbody.hpp>
#include <core/physics/collider.hpp>
#include <utilities/logging.hpp>


namespace Core {


struct Rigidbody::Impl
{
  Collider collider;
  
  uint32_t collision_id = -1;
  uint32_t mask_id = -1; // combine these two please
  
  float mass = 1;
};


Rigidbody::Rigidbody()
: m_impl(new Impl)
{
  LOG_TODO_ONCE("Rule of 5?");
}


Rigidbody::~Rigidbody()
{
}


Rigidbody::Rigidbody(const Rigidbody &other)
: m_impl(new Impl(*other.m_impl))
{
}


void
Rigidbody::set_collision_mask(const uint32_t rb_id, const uint32_t rb_collides_with_ids)
{
  assert(m_impl);
  m_impl->mask_id = rb_collides_with_ids;
  m_impl->collision_id = rb_id;
}


uint32_t
Rigidbody::get_rb_id() const
{
  assert(m_impl);
  return m_impl->collision_id;
}


uint32_t
Rigidbody::get_rb_mask() const
{
  assert(m_impl);
  return m_impl->mask_id;
}


Collider
Rigidbody::get_collider() const
{
  assert(m_impl);
  return m_impl->collider;
}


void
Rigidbody::set_collider(const Core::Collider &collider)
{
  assert(m_impl);
  m_impl->collider = collider;
}


float
Rigidbody::get_mass() const
{
  assert(m_impl);
  return m_impl->mass;
}


void
Rigidbody::set_mass(const float mass)
{
  assert(m_impl);
  m_impl->mass = mass;
}


} // ns