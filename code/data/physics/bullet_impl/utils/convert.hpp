/*!
  Converts physics types to and from bullet types
*/
#ifndef CONVERT_INCLUDED_D4A5E751_F8D5_40DA_A4E6_DD30430E54E7
#define CONVERT_INCLUDED_D4A5E751_F8D5_40DA_A4E6_DD30430E54E7


#include "../../config_physics.hpp"
#include "../../fwd.hpp"
#include "bullet_fwd.hpp"
#include <math/fwd.hpp>


namespace Bullet_detail {


btCollisionShape*
generate_collision_shape(
  const Data::Physics::Collider_config *collider,
  const uintptr_t user_data,
  const math::vec3 scale);


Data::Physics::Collider_config
generate_collider(
  const btCollisionShape *shape,
  const math::vec3 scale);


btTransform
generate_transform(
  const math::transform *trans,
  const math::aabb *aabb);


} // ns


#endif // inc guard
