#include <transformations/physics/bullet/core_rb_to_bullet_rb.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/physics/bullet/motion_state.hpp>
#include <transformations/physics/bullet/bullet_data.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/collider_utils.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <utilities/optimizations.hpp>
#include <math/geometry/aabb.hpp>
#include <assert.h>


namespace {


inline btCollisionShape*
generate_collision_shape(
  const Core::Collider &collider,
  const uintptr_t user_data,
  const math::vec3 scale)
{
  switch(collider.get_type())
  {
    case(Core::Collider::Type::box):
    {
      const btVector3 bt_half_extents(0.5f, 0.5f, 0.5f);
      
      btBoxShape *bt_collider = new btBoxShape(bt_half_extents);
   
      const Core::Box_collider core_box(Core::Collider_utis::cast_to_box_collider(collider));
      const math::vec3 half_extents = Core::Box_collider_utils::get_full_extents(core_box);
      const math::vec3 scaled_extents(math::vec3_multiply(half_extents, scale));
      const btVector3 scale_vector = math::vec3_to_bt(scaled_extents);
      
      bt_collider->setLocalScaling(scale_vector);
      bt_collider->setUserPointer((void*)user_data);
    
      return bt_collider;
    
      break;
    }
    
    default:
      assert(false);
  };
  
  UNREACHABLE;
  
  return nullptr;
}


inline Core::Collider
generate_collider(
  const btCollisionShape *shape,
  const math::vec3 entity_scale)
{
  const math::vec3 local_scale = math::vec3_from_bt(shape->getLocalScaling());
  const math::vec3 extents = math::vec3_divide(entity_scale, local_scale);
  
  return Core::Box_collider(math::get_x(extents), math::get_y(extents), math::get_z(extents));
}


inline btTransform
generate_transform(const Core::Transform *trans, const math::aabb *aabb)
{
  const math::vec3 origin(math::aabb_get_origin(*aabb));
  const math::vec3 offset(math::vec3_add(trans->get_position(), origin));
  
  return math::transform_to_bt(
    Core::Transform(
      offset,
      trans->get_scale(),
      trans->get_rotation()
    )
  );
}


} // anon ns


namespace Physics_transform {


void
create_core_rb_from_trigger(Core::Rigidbody *out_rb,
                            const math::vec3 entity_scale,
                            const Bullet_data::Trigger *trigger)
{
  // Param check
  assert(out_rb);
  assert(trigger);
  
  if(trigger->ghost_ptr)
  {
    out_rb->set_is_trigger(true);
    out_rb->set_is_kinematic(true);
    out_rb->set_mass(0);
    
    const btCollisionShape *shape = reinterpret_cast<btCollisionShape*>(trigger->shape_ptr);
    assert(shape);
    
    out_rb->set_collider(generate_collider(shape, entity_scale));
  }
}


void
create_core_rb_from_rigidbody(Core::Rigidbody *out_rb,
                              const math::vec3 entity_scale,
                              const Bullet_data::Rigidbody *rigidbody)
{
  // Param check
  assert(out_rb);
  assert(rigidbody);
  
  if(rigidbody)
  {
    out_rb->set_is_trigger(false);
    
    const btRigidBody *rb = reinterpret_cast<btRigidBody*>(rigidbody->rigidbody_ptr);
    assert(rb);
    
    out_rb->set_mass(1.f / rb->getInvMass());
    out_rb->set_is_kinematic(rb->getFlags() & btRigidBody::CF_KINEMATIC_OBJECT);
    
    const btCollisionShape *shape = rb->getCollisionShape();
    assert(shape);
    
    out_rb->set_collider(generate_collider(shape, entity_scale));
  }
}


void
create_trigger_from_core_rb(const Core::Transform *transform,
                            const math::aabb &aabb,
                            const Core::Rigidbody *core_rb,
                            Bullet_data::Trigger *out_trigger,
                            btDynamicsWorld *phy_world,
                            const uintptr_t user_data)
{
  // Param check
  assert(transform);
  assert(core_rb);
  assert(out_trigger);
  assert(phy_world);
  
  // Already exists? Then remove it
  if(out_trigger->ghost_ptr)
  {
    Bullet_data::remove_and_clear(out_trigger, phy_world);
  }
  
  // Create Collider
  btCollisionShape *bt_collider = nullptr;
  {
    bt_collider = generate_collision_shape(
      core_rb->get_collider(),
      user_data,
      math::vec3_multiply(
        transform->get_scale(),
        math::aabb_get_extents(aabb)
      )
    );
  }
  
  const btTransform bt_transform = generate_transform(transform, &aabb);
  
  // Create Trigger
  btPairCachingGhostObject *bt_trigger = nullptr;
  {
    bt_trigger = new btPairCachingGhostObject;
    bt_trigger->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
    bt_trigger->setWorldTransform(bt_transform);
    bt_trigger->setCollisionShape(bt_collider);
  }
  
  // Add to world
  {
    phy_world->addCollisionObject(bt_trigger);
  }
  
  // Set the values
  {
    assert(!out_trigger->ghost_ptr);
    out_trigger->ghost_ptr = (uintptr_t)bt_trigger;
    
    assert(!out_trigger->shape_ptr);
    out_trigger->shape_ptr = (uintptr_t)bt_collider;
  }
}


void
create_rigidbody_from_core_rb(const Core::Transform *transform,
                              const math::aabb &aabb,
                              const Core::Rigidbody *core_rb,
                              Bullet_data::Rigidbody *out_rb,
                              btDynamicsWorld *phy_world,
                              const uintptr_t user_data)
{
  // Param Check
  assert(transform);
  assert(core_rb);
  assert(out_rb);
  assert(phy_world);

  // Already exists? Then remove it
  if(out_rb->rigidbody_ptr)
  {    
    Bullet_data::remove_and_clear(out_rb, phy_world);
  }
  
  // Create Collider
  btCollisionShape *bt_collider = nullptr;
  {
    bt_collider = generate_collision_shape(
      core_rb->get_collider(),
      user_data,
      math::vec3_multiply(transform->get_scale(), math::aabb_get_extents(aabb))
    );
  }
  
  const btTransform bt_transform = generate_transform(transform, &aabb);
  
  // Create Rigidbody
  btRigidBody   *bt_rb = nullptr;
  btMotionState *bt_mt = nullptr;
  {
    uint32_t collision_flags(0);
    
    if(core_rb->is_kinematic())
    {
      collision_flags |= btRigidBody::CF_KINEMATIC_OBJECT;
    }
    
    if(core_rb->get_mass() == 0)
    {
      collision_flags |= btRigidBody::CF_STATIC_OBJECT;
    }

    // Create btRigidBody and return it.

    // If we don't have user data we can't update
    // transforms in core. The fallback here is unlikely to
    // be used but have it as a fallback incase.
    {
      if(user_data)
      {
        bt_mt = new Core_motion_state(
          user_data, bt_transform
        );
      }
      else
      {
        bt_mt = new btDefaultMotionState(
          bt_transform
        );
      }
    }
    
    
    const btScalar mass(core_rb->get_mass());
    btVector3 inertia(0.f, 0.f, 0.f);
    
    bt_collider->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rb_ci(mass, bt_mt, bt_collider, inertia);
    rb_ci.m_friction = 0.9;
    rb_ci.m_restitution = 0;

    bt_rb = new btRigidBody(rb_ci);
  }
  
  // Add to the world
  {
    phy_world->addRigidBody(bt_rb);
  }
  
  // Set the values
  {
    assert(!out_rb->rigidbody_ptr);
    out_rb->rigidbody_ptr = (uintptr_t)bt_rb;
    
    assert(!out_rb->motion_state_ptr);
    out_rb->motion_state_ptr = (uintptr_t)bt_mt;
    
    assert(!out_rb->shape_ptr);
    out_rb->shape_ptr = (uintptr_t)bt_collider;
  }
}


void
update_trigger_transform(Bullet_data::Trigger *trigger,
                         const btVector3 scale,
                         const btTransform *transform)
{
  // -- Param Check -- //
  assert(trigger);
  assert(transform);
  
  // -- Get Data -- //
  btGhostObject *ghost(
    reinterpret_cast<btGhostObject*>(trigger->ghost_ptr)
  );
  
  btCollisionShape *shape(
    ghost->getCollisionShape()
  );
  
  // -- Update Transforms -- //
  if(ghost && shape && transform)
  {
    ghost->setWorldTransform(*transform);
    shape->setLocalScaling(shape->getLocalScaling() * scale);
  }
}


void
update_rigidbody_transform(Bullet_data::Rigidbody *rigidbody,
                           Bullet_data::World *phy_world,
                           const math::aabb *aabb,
                           const btTransform *transform,
                           const btVector3 scale)
{
  // Param Check
  assert(rigidbody);
  assert(phy_world);
  assert(transform);
  
  btDynamicsWorld *world(
    phy_world->dynamics_world
  );
    
  btRigidBody *rb(
    reinterpret_cast<btRigidBody*>(rigidbody->rigidbody_ptr)
  );
  
  btCollisionShape *shape(
    rb->getCollisionShape()
  );
  
  // Update the transform and scale.
  if(rb && shape && world && transform)
  {
    // Save out the current state of things that shouldn't change
    const btVector3 curr_lin = rb->getLinearVelocity();
    const btVector3 curr_ang = rb->getAngularVelocity();
  
    btCollisionShape *shape = rb->getCollisionShape();
    assert(shape);

    // Update the transform
    {
      rb->setWorldTransform(*transform);
      shape->setLocalScaling(shape->getLocalScaling() * scale);
    }
    
    // Update the mass / inertia
    {
      const btScalar mass = 1.0 / rb->getInvMass();
      btVector3 inertia(0.f, 0.f, 0.f);
      
      shape->calculateLocalInertia(mass, inertia);
      rb->setMassProps(mass, inertia);
      rb->updateInertiaTensor();
      world->updateSingleAabb(rb);
    }
    
    // Restore state that was saved.
    rb->setLinearVelocity(curr_lin);
    rb->setAngularVelocity(curr_ang);
  }
}


} // ns
