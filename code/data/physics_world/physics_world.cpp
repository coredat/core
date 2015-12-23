#include "physics_world.hpp"
#include <data/entity/entity_data.hpp>
#include <assert.h>


namespace Data {


void
physics_world_init(Physics_world *world)
{
  assert(world);
  
  world->dynamics_world.setGravity(btVector3(0, -10, 0));
  //world->dynamics_world.setDebugDrawer(&debug_draw);
}


void
physics_world_step(Physics_world *world, const float dt)
{
  assert(world);

  world->dynamics_world.stepSimulation(dt / 1, 10);
}


void
physics_add_rigidbody(Physics_world *world, const Collider_detail collider, Rigidbody *out)
{
  const btScalar mass = 1.0;
  
  btVector3 extents(0.5, 0.5, 0.5);
  out->shape.reset(new btBoxShape(extents));
  
  btVector3 inertia(0, 0, 0);
  out->shape->calculateLocalInertia(mass, inertia);
  
  //rb.motion_state.reset(new Data_detail::Motion_state(entity, data));
  btRigidBody::btRigidBodyConstructionInfo rigidbody_ci(mass, out->motion_state.get(), out->shape.get(), inertia);
  
  out->rigidbody.reset(new btRigidBody(rigidbody_ci));
  
  world->dynamics_world.addRigidBody(out->rigidbody.get());
}



} // ns