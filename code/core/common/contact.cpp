#include <core/common/contact.hpp>


namespace Core {


Contact::Contact()
: Contact(math::vec3_zero(), math::vec3_zero_zero_one(), 0.f)
{}


Contact::Contact(const math::vec3 pos, const math::vec3 norm, const float pen_depth)
: m_position(pos)
, m_normal(norm)
, m_penetration_depth(pen_depth)
{
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


} // ns