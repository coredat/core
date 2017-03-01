#include "../physics.hpp"
#include "../config_physics.hpp"
#include "../config_rigidbody.hpp"
#include "../config_collider.hpp"
#include "utils/convert.hpp"
#include "utils/motion_state.hpp"
#include "utils/bullet_math_extensions.hpp"
#include "physics_data.hpp"
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>
#include <utilities/key.hpp>


namespace {

inline uint32_t
get_bt_collision_flags(const Data::Physics::Rigidbody_config *rb_conf)
{
  uint32_t collision_flags(0);

  if(rb_conf->is_kinematic)
  {
    collision_flags |= btRigidBody::CF_KINEMATIC_OBJECT;
  }
  if(rb_conf->mass == 0.f)
  {
    collision_flags |= btRigidBody::CF_STATIC_OBJECT;
  }
  
  return collision_flags;
}

} // anon ns


// ------------------------------------------------------[ Physics Lifetime ]--


namespace Data {
namespace Physics {


void
rigidbody_add(
  Physics_data *phys,
  const uint32_t id,
  const Rigidbody_config *rb,
  const Collider_config *collider,
  const math::transform *transform,
  const math::aabb *aabb)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(id);
  LIB_ASSERT(rb);
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
  
  // Generate motion state
  btMotionState *bt_mt(
    new Bullet_detail::Core_motion_state(
      id, bt_transform
    )
  );
  
  // Create Rigidbody
  btRigidBody *bt_rb = nullptr;
  {
    const btScalar mass(rb->mass);
    btVector3 inertia(0.f, 0.f, 0.f);
    
    bt_shape->calculateLocalInertia(mass, inertia);
    
    btRigidBody::btRigidBodyConstructionInfo rb_ci(
      mass,
      bt_mt,
      bt_shape,
      inertia
    );
    
    rb_ci.m_friction = 0.9;
    rb_ci.m_restitution = 0;

    bt_rb = new btRigidBody(rb_ci);
    bt_rb->setCollisionFlags(get_bt_collision_flags(rb));
  }
  
  // Check we have all we need
  {
    LIB_ASSERT(bt_rb);
    LIB_ASSERT(bt_mt);
    LIB_ASSERT(bt_shape);
  }
  
  // Create an internal object to track this.
  {
    Rigidbody internal_rb;
    internal_rb.rigidbody_ptr    = (uintptr_t)bt_rb;
    internal_rb.motion_state_ptr = (uintptr_t)bt_mt;
    internal_rb.shape_ptr        = (uintptr_t)bt_shape;
    
    LOG_TODO_ONCE(
      "We should insert these in id order, then we can binary search them."
    );
    
    phys->rb_ids.emplace_back(id);
    phys->rb_data.emplace_back(internal_rb);
  }
  
  // Be sure our rb containers are in sync.
  {
    LIB_ASSERT(phys->rb_ids.size() == phys->rb_data.size());
    LIB_ASSERT(phys->rb_ids.capacity() == phys->rb_data.capacity());
  }
  
  // Add to the world
  if(bt_rb)
  {
    phys->dynamics_world->addRigidBody(bt_rb);
  }
}


void
rigidbody_update(
  Physics_data *phys,
  const uint32_t id,
  const Rigidbody_config *rb_config,
  const Collider_config *collider,
  const math::transform *transform,
  const math::aabb *aabb)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(id);
  
  // Find RB //
  size_t index = 0;
  
  if(!lib::key::linear_search(
    id,
    phys->rb_ids.data(),
    phys->rb_ids.size(),
    &index))
  {
    LOG_ERROR("Rigidbody ID not found to be able to update.");
    LIB_ASSERT(false);
    return;
  }
  
  Rigidbody internal_rb = phys->rb_data[index];
  
  btRigidBody *bt_rb = (btRigidBody*)internal_rb.rigidbody_ptr;
  LIB_ASSERT(bt_rb);
  
  // Remove from the world while we update it.
  phys->dynamics_world->removeRigidBody(bt_rb);
  
  // Config Update
  if(rb_config)
  {
    btVector3 inertia(0.f,0.f,0.f);
    bt_rb->setMassProps(rb_config->mass, inertia);
    bt_rb->setCollisionFlags(get_bt_collision_flags(rb_config));
  }
  
  // Collider Update
  if(collider)
  {
    // Hot swap not supported yet.
//    LIB_ASSERT(false);
  }
  
  // Transform Update
  LIB_ASSERT(transform && aabb); // Need both to update transform
  
  if(transform && aabb)
  {
    bt_rb->setCenterOfMassTransform(
      Bullet_detail::generate_transform(transform, aabb)
    );
    
    bt_rb->getCollisionShape()->setLocalScaling(
      math::vec3_to_bt(
        math::vec3_multiply(
          transform->scale,
          math::aabb_get_extents(*aabb)
        )
      )
    );
  }
  
  // Add back to the world.
  phys->dynamics_world->addRigidBody(bt_rb);
  
}


void
rigidbody_remove(
  Physics_data *phys,
  const uint32_t id)
{
}


} // ns
} // ns
