#include <core/common/collision.hpp>


namespace Core {


Collision::Collision(const Entity_ref ref,
                     const math::vec3 pos,
                     const math::vec3 norm)
: m_object(ref)
, m_position(pos)
, m_normal(norm)
{
}


Entity_ref
Collision::get_entity() const
{
  return m_object;
}


math::vec3
Collision::get_normal() const
{
  return m_normal;
}


math::vec3
Collision::get_position() const
{
  return m_position;
}


bool
Collision::has_hit() const
{
  return !!m_object;
}


Collision::operator bool() const
{
  return has_hit();
}


} // ns