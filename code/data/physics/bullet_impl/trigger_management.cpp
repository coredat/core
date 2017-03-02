#include "../physics.hpp"
#include "../config_physics.hpp"
#include "../config_trigger.hpp"
#include "../config_collider.hpp"
#include "utils/convert.hpp"
#include "utils/motion_state.hpp"
#include "physics_data.hpp"
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>
#include <utilities/key.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


// ------------------------------------------------------[ Physics Lifetime ]--


namespace Data {
namespace Physics {


void
trigger_add(
  Physics_data *phys,
  const uint32_t id,
  const Trigger_config *trig,
  const Collider_config *collider,
  const math::transform *transform,
  const math::aabb *aabb)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(id);
  LIB_ASSERT(trig);
  LIB_ASSERT(collider);
  LIB_ASSERT(transform);
  LIB_ASSERT(aabb);
  
  #ifdef PHYSICS_PEDANTIC_ERROR_CHECKS
  {
    if(lib::key::linear_search(
      id,
      phys->rb_ids.data(),
      phys->rb_ids.size()))
    {
      LOG_ERROR("This Entity already has a rigidbody.");
      return;
    }
    
    if(lib::key::linear_search(
      id,
      phys->trigger_ids.data(),
      phys->trigger_ids.size()))
    {
      LOG_ERROR("This Entity already has a trigger.");
      return;
    }
  }
  #endif
  
  // Generate Collider
  btCollisionShape *bt_shape(
    Bullet_detail::generate_collision_shape(
      collider,
      id,
      math::vec3_multiply(
        transform->scale,
        math::aabb_get_extents(*aabb)
      )
    )
  );
  
  // Generate transform
  const btTransform bt_transform(
    Bullet_detail::generate_transform(transform, aabb)
  );

  LOG_TODO_ONCE(
    "Dry! All steps up to here are same as rb!"
  );
  
  // Create Trigger
  btPairCachingGhostObject *bt_trigger = nullptr;
  {
    bt_trigger = new btPairCachingGhostObject;
    bt_trigger->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
    bt_trigger->setWorldTransform(bt_transform);
    bt_trigger->setCollisionShape(bt_shape);
  }
  
  // Check we have all we need
  {
    LIB_ASSERT(bt_trigger);
    LIB_ASSERT(bt_shape);
  }
  
  // Create an internal object to track this.
  {
    Trigger internal_trigger;
    internal_trigger.ghost_ptr = (uintptr_t)bt_trigger;
    internal_trigger.shape_ptr = (uintptr_t)bt_shape;
    
    LOG_TODO_ONCE(
      "We should insert these in id order, then we can binary search them."
    );
    
    phys->trigger_ids.emplace_back(id);
    phys->trigger_data.emplace_back(internal_trigger);
  }
  
  // Be sure our rb containers are in sync.
  {
    LIB_ASSERT(phys->trigger_ids.size() == phys->trigger_data.size());
    LIB_ASSERT(phys->trigger_ids.capacity() == phys->trigger_data.capacity());
  }
  
  // Add to world
  if(bt_trigger)
  {
    phys->dynamics_world->addCollisionObject(bt_trigger);
  }
}


void
trigger_remove(Physics_data *phys, const uint32_t id)
{
}


void
trigger_udpate(
  Physics_data *phys,
  const uint32_t id,
  const Trigger_config *trig,
  const Collider_config *collider,
  const math::transform *transform,
  const math::aabb *aabb)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(id);

  // not impl yet.
  LIB_ASSERT(false);
}


} // ns
} // ns
