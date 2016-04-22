#include <core/physics/box_collider_utils.hpp>
#include <core/physics/box_collider.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Box_collider_utils {


math::vec3
get_full_extents(const Box_collider coll)
{
  return math::vec3_init(coll.get_x_half_extent() * 2.f,
                         coll.get_y_half_extent() * 2.f,
                         coll.get_z_half_extent() * 2.f);
}


math::vec3
get_half_extents(const Box_collider coll)
{
  return math::vec3_init(coll.get_x_half_extent(),
                         coll.get_y_half_extent(),
                         coll.get_z_half_extent());
}


Box_collider
create_with_full_extents(const math::vec3 extents)
{
  return Box_collider(math::vec3_get_x(extents) * 0.5f,
                      math::vec3_get_y(extents) * 0.5f,
                      math::vec3_get_z(extents) * 0.5f);
}


Box_collider
create_with_half_extents(const math::vec3 half_extents)
{
  return Box_collider(math::vec3_get_x(half_extents),
                      math::vec3_get_y(half_extents),
                      math::vec3_get_z(half_extents));
}


} // ns
} // ns
