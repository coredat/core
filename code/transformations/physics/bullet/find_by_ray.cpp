#include <transformations/physics/bullet/find_by_ray.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <data/memory/memory_data.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/common/ray.hpp>
#include <core/common/collision.hpp>
#include <math/vec/vec3.hpp>
#include <btBulletDynamicsCommon.h>


namespace Physics_transform {


Core::Contact
find_entity_from_ray(const Core::Ray ray,
                     btDiscreteDynamicsWorld *phy_world)
{
  // Param check
  assert(phy_world);

  const btVector3 start_pos(math::vec3_to_bt(ray.get_origin()));
  const btVector3 end_pos(math::vec3_to_bt(math::vec3_add(ray.get_origin(),
                                           math::vec3_scale(ray.get_direction(),
                                           btScalar(ray.get_distance())))));
  
  btCollisionWorld::ClosestRayResultCallback ray_callback(
    start_pos, end_pos
  );
  
  phy_world->rayTest(start_pos, end_pos, ray_callback);
  
  Core::Contact contact;

  if(ray_callback.hasHit())
  {
    // Length from ray origin to the hit point.
    const float length(
      math::vec3_length(
        math::vec3_subtract(
          ray.get_origin(),
          math::vec3_from_bt(ray_callback.m_hitPointWorld)
        )
      )
    );
  
    const uintptr_t user_data(
      (uintptr_t)ray_callback.m_collisionObject->getCollisionShape()->getUserPointer()
    );
    
    contact = Core::Contact(
      Core::Entity_ref(
        Core_detail::entity_id_from_uint(user_data)
      ),
      math::vec3_from_bt(ray_callback.m_hitPointWorld),
      math::vec3_from_bt(ray_callback.m_hitNormalWorld),
      length
    );
  }
  
  return contact;
}


void
find_entities_from_ray(const Core::Ray ray,
                       btDiscreteDynamicsWorld *phy_world,
                       Core::Contact **volatile_contact_results,
                       size_t *out_contact_count)
{
  // Param check
  assert(phy_world);
  
  const btVector3 start_pos(math::vec3_to_bt(ray.get_origin()));
  const btVector3 end_pos(math::vec3_to_bt(math::vec3_add(ray.get_origin(),
                                           math::vec3_scale(ray.get_direction(),
                                           btScalar(ray.get_distance())))));
  
  btCollisionWorld::AllHitsRayResultCallback ray_callback(start_pos, end_pos);
  
  phy_world->rayTest(start_pos, end_pos, ray_callback);
  
  if(ray_callback.hasHit())
  {
    int hit_count = ray_callback.m_collisionObjects.size();
    *out_contact_count = hit_count;
  
    Core::Contact *contacts = (Core::Contact*)SCRATCH_ALIGNED_ALLOC(Core::Contact, hit_count);
    *volatile_contact_results = contacts;
  
    for(int i = 0; i < hit_count; ++i)
    {
      const btCollisionObject *obj = ray_callback.m_collisionObjects.at(i);
    
      // Rays shouldn't pen, so we can default it.
      constexpr float penitration_depth(0.f);
    
      const uintptr_t user_data = (uintptr_t)obj->getCollisionShape()->getUserPointer();
      
      new (&contacts[i]) Core::Contact(Core::Entity_ref(Core_detail::entity_id_from_uint(user_data)),
                                       math::vec3_from_bt(ray_callback.m_hitPointWorld.at(i)),
                                       math::vec3_from_bt(ray_callback.m_hitNormalWorld.at(i)),
                                       penitration_depth);
    }
  }
}


} // ns