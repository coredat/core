#include <core/physics/box_collider_utils.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/collider.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Box_collider_utils {


Collider
cast_to_collider(const Box_collider coll)
{
  return Collider(Core::Collider::Type::box,
                  coll.get_x_extent(),
                  coll.get_y_extent(),
                  coll.get_z_extent());
}


math::vec3
get_full_extents(const Box_collider coll)
{
  return math::vec3_init(coll.get_x_extent(),
                         coll.get_y_extent(),
                         coll.get_z_extent());
}


math::vec3
get_half_extents(const Box_collider coll)
{
  return math::vec3_init(coll.get_x_extent() * 0.5f,
                         coll.get_y_extent() * 0.5f,
                         coll.get_z_extent() * 0.5f);
}


Box_collider
create_with_full_extents(const math::vec3 extents)
{
  return Box_collider(math::vec3_get_x(extents),
                      math::vec3_get_y(extents),
                      math::vec3_get_z(extents));
}


Box_collider
create_with_half_extents(const math::vec3 half_extents)
{
  return Box_collider(math::vec3_get_x(half_extents) * 0.5f,
                      math::vec3_get_y(half_extents) * 0.5f,
                      math::vec3_get_z(half_extents) * 0.5f);
}


float
get_volume(const Box_collider collider)
{
  return collider.get_x_extent() *
         collider.get_y_extent() *
         collider.get_z_extent();
}


} // ns
} // ns
