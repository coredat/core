#ifndef BOX_COLLIDER_UTILS_INCLUDED_380A8105_B7D3_4D6C_9E2F_1C6171CE1ACD
#define BOX_COLLIDER_UTILS_INCLUDED_380A8105_B7D3_4D6C_9E2F_1C6171CE1ACD


#include <core/physics/physics_fwd.hpp>
#include <math/vec/vec_types.hpp>


namespace Core {
namespace Box_collider_utils {


math::vec3
get_full_extents(const Box_collider collider);


math::vec3
get_half_extents(const Box_collider collider);


Box_collider
create_with_full_extents(const math::vec3 extents);


Box_collider
create_with_half_extents(const math::vec3 half_extents);


} // ns
} // ns


#endif // inc guard