#ifndef BOX_COLLIDER_UTILS_INCLUDED_380A8105_B7D3_4D6C_9E2F_1C6171CE1ACD
#define BOX_COLLIDER_UTILS_INCLUDED_380A8105_B7D3_4D6C_9E2F_1C6171CE1ACD


#include <core/physics/physics_fwd.hpp>
#include <math/vec/vec_fwd.hpp>


namespace Core {
namespace Box_collider_utils {


/*!
  An explicit cast to collider type.
*/
Collider
cast_to_collider(const Box_collider collider);


/*!
  Get a math::vec3 of the full extent.
*/
math::vec3
get_full_extents(const Box_collider collider);


/*!
  Get a math::vec3 of the half extent.
*/
math::vec3
get_half_extents(const Box_collider collider);


/*!
  Create a box collider with the full extent math::vec3.
*/
Box_collider
create_with_full_extents(const math::vec3 extents);


/*!
  Create a box collider with a half extent math::vec3.
*/
Box_collider
create_with_half_extents(const math::vec3 half_extents);


/*!
  Returns the volume of the collider
*/
float
get_volume(const Box_collider collider);


} // ns
} // ns


#endif // inc guard