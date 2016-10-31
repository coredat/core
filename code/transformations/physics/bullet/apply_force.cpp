#include "apply_force.hpp"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <LinearMath/btVector3.h>


namespace Physics_transform {


void
apply_world_force(btRigidBody *rb,
                  const btVector3 dir,
                  const btScalar power)
{
  const btVector3 force = dir * power;
  const btVector3 rel_pos(0,0,0);

  rb->activate(true);
  rb->applyForce(force, rel_pos);
}


} // ns