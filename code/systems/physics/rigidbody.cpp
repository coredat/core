#include "rigidbody.hpp"
#include <assert.h>


namespace Physics {


void
rigidbody_apply_local_force(Rigidbody *rb, const float force[3])
{
  // Param check
  assert(rb && rb->rigidbody);
  
  const auto bt_rb = rb->rigidbody.get();

  const btVector3 world_force(force[0], force[1], force[2]);
  const btVector3 local_force(bt_rb->getWorldTransform().getBasis() * world_force);
  
  const btVector3 rel_pos(0, 0, 0);
  
  bt_rb->activate(true);
  bt_rb->applyForce(local_force, rel_pos);
}


void
rigidbody_apply_local_torque(Rigidbody *rb, const float torque[3])
{
  // Param check
  assert(rb && rb->rigidbody);
  
  const auto bt_rb = rb->rigidbody.get();

  const btVector3 w_torque = btVector3(torque[0], torque[1], torque[2]);
  const btVector3 l_torque = bt_rb->getWorldTransform().getBasis() * w_torque;

  bt_rb->activate(true);
  bt_rb->applyTorque(l_torque);
}


} // ns