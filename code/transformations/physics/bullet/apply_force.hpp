#ifndef APPLY_FORCE_INCLUDED_0D02785F_A07B_40A8_8E61_82F26E190AB4
#define APPLY_FORCE_INCLUDED_0D02785F_A07B_40A8_8E61_82F26E190AB4


#include <LinearMath/btScalar.h>
class btRigidBody;
class btVector3;


namespace Physics_transform {


void
apply_world_force(btRigidBody *rb,
                  const btVector3 dir,
                  const btScalar power);


} // ns


#endif // inc guard