#include <transformations/physics/bullet/find_by_ray.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/common/ray.hpp>
#include <core/common/collision.hpp>
#include <math/vec/vec3.hpp>
#include <btBulletDynamicsCommon.h>



namespace Physics_transform {


Core::Collision
find_entity_from_ray(const Core::Ray ray,
                     btDiscreteDynamicsWorld *phy_world)
{
  // Param check
  assert(phy_world);

  const btVector3 start_pos(math::vec3_to_bt(ray.get_origin()));
  const btVector3 end_pos(math::vec3_to_bt(math::vec3_add(ray.get_origin(), math::vec3_scale(ray.get_direction(), ray.get_distance()))));
  
  btCollisionWorld::ClosestRayResultCallback ray_callback(start_pos, end_pos);
  
  phy_world->rayTest(start_pos, end_pos, ray_callback);
  
  Core::Collision collision;
  Core::Contact contact;

  if(ray_callback.hasHit())
  {
    // Rays shouldn't pen, so we can default it.
    constexpr float penitration_depth(0.f);
  
    contact = Core::Contact(math::vec3_from_bt(ray_callback.m_hitPointWorld),
                                               math::vec3_from_bt(ray_callback.m_hitNormalWorld),
                                               penitration_depth);
    
    const uintptr_t user_data = (uintptr_t)ray_callback.m_collisionObject->getCollisionShape()->getUserPointer();
    collision = Core::Collision(Core::Entity_ref(Core_detail::entity_id_from_uint(user_data)), &contact, 1);
  }
  
  return collision;
}


} // ns