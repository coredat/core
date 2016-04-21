#include <core/physics/collider_utils.hpp>
#include <core/physics/box_collider.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Collider_utis {


Box_collider
create_box_collider(const math::vec3 extents)
{
  return Box_collider(math::vec3_get_x(extents),
                      math::vec3_get_y(extents),
                      math::vec3_get_z(extents));
}



} // ns
} // ns