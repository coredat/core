#ifndef CORE_RB_TO_BULLET_RB_INCLUDED_72F9C618_8B95_4D3E_BCD2_6DB50014B698
#define CORE_RB_TO_BULLET_RB_INCLUDED_72F9C618_8B95_4D3E_BCD2_6DB50014B698


#include <core/common/core_fwd.hpp>

class btRigidBody;
class btTransform;
class btCollisionShape;


namespace Physics_transform {


btRigidBody*
convert_core_rb_to_bullet_rb(const Core::Rigidbody *core_rb,
                             btCollisionShape *collider,
                             const btTransform *transform);
  
  
btCollisionShape*
convert_core_collider_to_bullet_collider(const Core::Collider *collider,
                                         const Core::Transform *transform,
                                         const uintptr_t user_data = 0);


} // ns


#endif // inc guard