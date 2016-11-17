#ifndef CORE_RB_TO_BULLET_RB_INCLUDED_72F9C618_8B95_4D3E_BCD2_6DB50014B698
#define CORE_RB_TO_BULLET_RB_INCLUDED_72F9C618_8B95_4D3E_BCD2_6DB50014B698


#include <core/common/core_fwd.hpp>
#include <transformations/physics/bullet/bullet_fwd.hpp>


namespace Physics_transform {


/*!
  Takes a core rigidbody definition and adds it to the
  physics world as a rigidbody.
  
  This function will remove any rigidbody identififed in
  out_rb.
*/
void
create_rigidbody_from_core_rb(const Core::Transform *transform,
                              const Core::Rigidbody *core_rb,
                              Bullet_data::Rigidbody *out_rb,
                              btDynamicsWorld *phy_world,
                              const uintptr_t user_data = 0);
  

/*!
  Takes a core rigidbody (trigger) definition and adds it to the
  physics world as a trigger.
  
  This function will remove any trigger identififed in
  out_rb.
*/
void
create_trigger_from_core_rb(const Core::Transform *transform,
                            const Core::Rigidbody *core_rb,
                            Bullet_data::Trigger *out_trigger,
                            btDynamicsWorld *phy_world,
                            const uintptr_t user_data = 0);


/*!
  Pulls the information from Bullet to convert it to a
  Core::Rigidbody.
*/
void
create_core_rb_from_trigger(Core::Rigidbody *out_rb,
                            const math::vec3 entity_scale,
                            const Bullet_data::Trigger *trigger);
  
  
/*!
  Pulls the information from Bullet to convert it to a
  Core::Rigidbody.
*/
void
create_core_rb_from_rigidbody(Core::Rigidbody *out_rb,
                              const math::vec3 entity_scale,
                              const Bullet_data::Rigidbody *rigidbody);


/*!
  Updates the transform of a trigger.
*/
void
update_trigger_transform(btPairCachingGhostObject *trigger,
                         const btTransform *transform);


/*!
  Updates the transform of a rigidbody. The function
  will try and preserve the angular/linear velocities.
*/
void
update_rigidbody_transform(btRigidBody *rb,
                           btDynamicsWorld *world,
                           const btTransform *transform,
                           const btVector3 scale);
  
} // ns


#endif // inc guard