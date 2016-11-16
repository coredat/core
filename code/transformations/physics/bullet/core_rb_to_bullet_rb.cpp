#include <transformations/physics/bullet/core_rb_to_bullet_rb.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <transformations/physics/bullet/motion_state.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/collider_utils.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <utilities/optimizations.hpp>
#include <assert.h>


namespace Physics_transform {


btRigidBody*
convert_core_rb_to_bullet_rb(const Core::Rigidbody *core_rb,
                             btCollisionShape *collider,
                             const btTransform *transform,
                             const uintptr_t user_data)
{
  // Param Check
  assert(core_rb);
  assert(!core_rb->is_trigger()); // Call to_bullet_trigger function.
  assert(collider);
  assert(transform);
  
  uint32_t collision_flags(0);
  
  if(core_rb->is_kinematic())   { collision_flags |= btRigidBody::CF_KINEMATIC_OBJECT; }
  if(core_rb->get_mass() == 0)  { collision_flags |= btRigidBody::CF_STATIC_OBJECT;    }

  // Create btRigidBody and return it.

  // If we don't have user data we can't update
  // transforms in core. The fallback here is unlikely to
  // be used but have it as a fallback incase.
  btDefaultMotionState *motion_state = nullptr;
  {
    if(user_data)
    {
      motion_state = new Core_motion_state(user_data, *transform);
    }
    else
    {
      motion_state = new btDefaultMotionState(*transform);
    }
  }
  
  
  const btScalar mass(core_rb->get_mass());
  btVector3 inertia(0.f, 0.f, 0.f);
  
  collider->calculateLocalInertia(mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo rb_ci(mass, motion_state, collider, inertia);
  rb_ci.m_friction = 0.9;
  rb_ci.m_restitution = 0;


  btRigidBody *return_rigidbody(new btRigidBody(rb_ci));
  
  return return_rigidbody;
}


btPairCachingGhostObject*
convert_core_rb_to_bullet_trigger(const Core::Rigidbody *core_rb,
                                  btCollisionShape *collider,
                                  const btTransform *transform)
{
  // Param Check
  assert(core_rb);
  assert(core_rb->is_trigger()); // If not a tirgger call to_bullet_rb
  assert(collider);
  assert(transform);
  
  if(core_rb && collider && transform)
  {
    btPairCachingGhostObject *return_trigger(new btPairCachingGhostObject);
    return_trigger->setCollisionFlags(btGhostObject::CF_NO_CONTACT_RESPONSE);
    return_trigger->setWorldTransform(*transform);
    return_trigger->setCollisionShape(collider);
    
    return return_trigger;
  }
  
  return nullptr;
}


void
update_trigger_transform(btPairCachingGhostObject *ghost,
                         const btTransform *transform)
{
  // Param Check
  assert(ghost);
  assert(transform);
  
  // Update Transform
  if(ghost && transform)
  {
    ghost->setWorldTransform(*transform);
  }
}


void
update_rigidbody_transform(btRigidBody *rb,
                           btDynamicsWorld *world,
                           const btTransform *transform,
                           const btVector3 scale)
{
  btCollisionShape *shape = rb->getCollisionShape();

  // Param Check
  assert(rb);
  assert(shape);
  assert(world);
  assert(transform);
  
  // Update the transform and scale.
  if(rb && shape && world && transform)
  {
    const btVector3 curr_lin = rb->getLinearVelocity();
    const btVector3 curr_ang = rb->getAngularVelocity();
  
    rb->setWorldTransform(*transform);
    rb->getCollisionShape()->setLocalScaling(scale);
    
    world->updateSingleAabb(rb);
    
    const btScalar mass = 1.0 / rb->getInvMass();
    btVector3 inertia(0.f, 0.f, 0.f);
    
    rb->getCollisionShape()->calculateLocalInertia(mass, inertia);
    rb->setMassProps(mass, inertia);

    rb->updateInertiaTensor();
    rb->setLinearVelocity(curr_lin);
    rb->setAngularVelocity(curr_ang);
  }
}


btCollisionShape*
convert_core_collider_to_bullet_collider(const Core::Collider *collider,
                                         const Core::Transform *transform,
                                         const uintptr_t user_data)
{
  // Param Check
  assert(collider);
  assert(transform);
  assert(user_data);
  
  if(!collider || !transform || !user_data)
  {
    return nullptr;
  }
  
  // Create btCollisionShape and return it.
  switch(collider->get_type())
  {
    case(Core::Collider::Type::box):
    {
      btBoxShape *box_shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
   
      const Core::Box_collider core_box(Core::Collider_utis::cast_to_box_collider(*collider));         
      const math::vec3 half_extents(
        math::vec3_init(
          core_box.get_x_half_extent(),
          core_box.get_y_half_extent(),
          core_box.get_z_half_extent()
        )
      );
      const math::vec3 scaled_extents(math::vec3_multiply(half_extents,
                                                          transform->get_scale()));
      const btVector3 scale_vector = math::vec3_to_bt(scaled_extents);
      
      box_shape->setLocalScaling(scale_vector);
      box_shape->setUserPointer((void*)user_data);
    
      return box_shape;
    }
    
    default:
      assert(false);
      return nullptr;
  };

  UNREACHABLE;
  
  return nullptr;
}



} // ns