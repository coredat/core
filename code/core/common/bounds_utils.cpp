#include <core/common/bounds_utils.hpp>
#include <math/vec/vec3.hpp>
#include <assert.h>


namespace Core {
namespace Build_utils {


Bounds
create_with_origin_extents(const math::vec3 origin, const math::vec3 extents)
{
  assert(false);
  return Bounds();
}


math::vec3
get_origin(const Bounds &bounds)
{
  assert(false);
  return math::vec3_subtract(bounds.get_max(), bounds.get_min());
}


math::vec3
get_extents(const Bounds &bounds)
{
  assert(false);
  return math::vec3_subtract(bounds.get_max(), bounds.get_min());
}


math::vec3
get_half_extents(const Bounds &bounds)
{
  assert(false);
  return math::vec3_subtract(bounds.get_max(), bounds.get_min());
}


} // ns
} // ns