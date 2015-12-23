#include "physics_world.hpp"
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
physics_add_rigidbody(Physics_world *world, const Entity_id entity, const Data::Entity_data *data)
{
  // TODO: mem leak I know, just want to get this working.
  
}



} // ns