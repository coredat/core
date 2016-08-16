#include <core/common/ray.hpp>
#include <math/vec/vec3.hpp>


namespace Core {


Ray::Ray(const math::vec3 origin, const math::vec3 direction)
: m_origin(origin)
, m_direction(math::vec3_normalize(direction))
{
}


math::vec3
Ray::get_direction() const
{
  return m_direction;
}


math::vec3
Ray::get_origin() const
{
  return m_origin;
}


} // ns