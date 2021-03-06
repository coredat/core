#include <core/common/plane.hpp>
#include <math/vec/vec3.hpp>


namespace Core {


Plane::Plane(const math::vec3 position, const math::vec3 normal)
: m_position(position)
, m_normal(math::vec3_normalize(normal))
{
}


math::vec3
Plane::get_position() const
{
  return m_position;
}


math::vec3
Plane::get_normal() const
{
  return m_normal;
}



} // ns