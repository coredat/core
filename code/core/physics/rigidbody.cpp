#include <core/physics/rigidbody.hpp>
#include <utilities/logging.hpp>


namespace Core {


//struct Rigidbody::Impl
//{
//
//};


Rigidbody::Rigidbody()
//: m_impl(new Impl)
{
  LOG_TODO_ONCE("Rule of 5?");
}


Rigidbody::Rigidbody(
  const Collider &collider,
  const float mass,
  const bool is_trigger)
: m_collider(collider)
, m_mass(mass)
, m_is_trigger(is_trigger)
{
}


Rigidbody::~Rigidbody()
{
}


Rigidbody::Rigidbody(const Rigidbody &other)
//: m_impl(new Impl(*other.m_impl))
{
}


void
Rigidbody::set_collision_mask(const uint32_t rb_id, const uint32_t rb_collides_with_ids)
{
//  assert(m_impl);
  m_mask_id = rb_collides_with_ids;
  m_collision_id = rb_id;
}


uint32_t
Rigidbody::get_rb_id() const
{
//  assert(m_impl);
  return m_collision_id;
}


uint32_t
Rigidbody::get_rb_mask() const
{
//  assert(m_impl);
  return m_mask_id;
}


Collider
Rigidbody::get_collider() const
{
//  assert(m_impl);
  return m_collider;
}


void
Rigidbody::set_collider(const Core::Collider &collider)
{
//  assert(m_impl);
  m_collider = collider;
}


float
Rigidbody::get_mass() const
{
//  assert(m_impl);
  return m_mass;
}


void
Rigidbody::set_mass(const float mass)
{
//  assert(m_impl);
  m_mass = mass;
}


bool
Rigidbody::is_kinematic() const
{
//  assert(m_impl);
  return !m_is_dynamic;
}


void
Rigidbody::set_is_kinematic(const bool set)
{
//  assert(m_impl);
  m_is_dynamic = set;
}


bool
Rigidbody::is_trigger() const
{
//  assert(m_impl);
  return m_is_trigger;
}


void
Rigidbody::set_is_trigger(const bool set)
{
//  assert(m_impl);
  m_is_trigger = set;
}


} // ns
