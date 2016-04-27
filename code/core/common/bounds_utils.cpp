#include <core/common/bounds_utils.hpp>
#include <math/vec/vec3.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Core {
namespace Bound_utils {


Bounds
create_with_origin_extents(const math::vec3 origin, const math::vec3 extents)
{
  const math::vec3 half_extents = math::vec3_scale(extents, 0.5f);
  
  return create_with_origin_half_extents(origin, half_extents);
}


Bounds
create_with_origin_half_extents(const math::vec3 origin, const math::vec3 half_extents)
{
  const math::vec3 min = math::vec3_subtract(origin, half_extents);
  const math::vec3 max = math::vec3_add(origin, half_extents);
  
  return Bounds(min, max);
}


math::vec3
get_origin(const Bounds &bounds)
{
  return math::vec3_add(bounds.get_min(), get_half_extents(bounds));
}


math::vec3
get_extents(const Bounds &bounds)
{
  return math::vec3_subtract(bounds.get_max(), bounds.get_min());
}


math::vec3
get_half_extents(const Bounds &bounds)
{
  return math::vec3_scale(get_extents(bounds), 0.5f);
}


bool
are_intersecting(const Bounds &a, const Bounds &b)
{
  LOG_TODO("Changes to math::aabb need to happen first");
  assert(false);
}


} // ns
} // ns