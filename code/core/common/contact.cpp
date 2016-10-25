#include <core/common/contact.hpp>


namespace Core {


Contact::Contact()
: Contact(Entity_ref(), math::vec3_zero(), math::vec3_zero_zero_one(), 0.f)
{}


Contact::Contact(const Entity_ref contact_object,
                 const math::vec3 pos,
                 const math::vec3 norm,
                 const float pen_depth)
: m_contact_object(contact_object)
, m_position(pos)
, m_normal(norm)
, m_penetration_depth(pen_depth)
{
}


Entity_ref
Contact::get_object() const
{
  return m_contact_object;
}


math::vec3
Contact::get_normal() const
{
  return m_normal;
}


math::vec3
Contact::get_position() const
{
  return m_position;
}


float
Contact::get_penetration_depth() const
{
  return m_penetration_depth;
}


Contact::operator bool() const
{
  return m_contact_object.is_valid();
}


} // ns