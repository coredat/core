#include <core/common/bounds.hpp>
#include <math/vec/vec3.hpp>


namespace Core {


Bounds::Bounds()
: Bounds(math::vec3_zero(), math::vec3_zero())
{
}


Bounds::Bounds(const math::vec3 min, const math::vec3 max)
: m_min(min)
, m_max(max)
{
}


math::vec3
Bounds::get_min() const
{
  return m_min;
}


math::vec3
Bounds::get_max() const
{
  return m_max;
}


} // ns