#ifndef CORE_RB_TO_BULLET_RB_INCLUDED_72F9C618_8B95_4D3E_BCD2_6DB50014B698
#define CORE_RB_TO_BULLET_RB_INCLUDED_72F9C618_8B95_4D3E_BCD2_6DB50014B698


#include <core/common/core_fwd.hpp>
#include <transformations/physics/bullet/bullet_data.hpp>


class btRigidBody;
class btPairCachingGhostObject;
class btDynamicsWorld;
class btTransform;
class btCollisionShape;
class btVector3;


namespace Physics_transform {


void
create_rigidbody_from_core_rb(const Core::Transform *transform,
                              const Core::Rigidbody *core_rb,
                              Bullet_data::Rigidbody *out_rb,
                              btDynamicsWorld *phy_world,
                              const uintptr_t user_data = 0);
  
  
void
create_trigger_from_core_rb(const Core::Transform *transform,
                            const Core::Rigidbody *core_rb,
                            Bullet_data::Trigger *out_trigger,
                            btDynamicsWorld *phy_world,
                            const uintptr_t user_data = 0);


void
create_core_rb_from_trigger(Core::Rigidbody *out_rb,
                            const math::vec3 entity_scale,
                            const Bullet_data::Trigger *trigger);
  
  
void
create_core_rb_from_rigidbody(Core::Rigidbody *out_rb,
                              const math::vec3 entity_scale,
                              const Bullet_data::Rigidbody *rigidbody);



//btRigidBody*
//convert_core_rb_to_bullet_rb(const Core::Rigidbody *core_rb,
//                             btCollisionShape *collider,
//                             const btTransform *transform,
//                             const uintptr_t user_data = 0);
  
  
//Core::Rigidbody
//convert_rb_to_core_rb(const btRigidBody *rb,
//                      const math::vec3 entity_global_scale);
//
//
//btPairCachingGhostObject*
//convert_core_rb_to_bullet_trigger(const Core::Rigidbody *core_rb,
//                                  btCollisionShape *collider,
//                                  const btTransform *transform);

void
update_trigger_transform(btPairCachingGhostObject *trigger,
                         const btTransform *transform);


void
update_rigidbody_transform(btRigidBody *rb,
                           btDynamicsWorld *world,
                           const btTransform *transform,
                           const btVector3 scale);
  


//btCollisionShape*
//convert_core_collider_to_bullet_collider(const Core::Collider *collider,
//                                         const Core::Transform *transform,
//                                         const uintptr_t user_data = 0);

} // ns


#endif // inc guard