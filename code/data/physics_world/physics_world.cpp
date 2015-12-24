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
  assert(world && out);
  
  switch(collider.type)
  {
    case(Collider_type::static_plane):
    {
      const btVector3 normal(0, 1, 0);
      const btScalar offset(0);
      out->shape.reset(new btStaticPlaneShape(normal, offset));
      break;
    }
    
    case(Collider_type::cube):
    {
      const btVector3 extents(collider.collider_info.cube.extents[0],
                              collider.collider_info.cube.extents[1],
                              collider.collider_info.cube.extents[2]);
      out->shape.reset(new btBoxShape(extents));
      break;
    }
    
    case(Collider_type::unknown):
    default:
      assert(false); // oh no you didn't.
      return;
  }
  
  btVector3 inertia(0, 0, 0);
  out->shape->calculateLocalInertia(collider.mass, inertia);
  
  //rb.motion_state.reset(new Data_detail::Motion_state(entity, data));
  const btRigidBody::btRigidBodyConstructionInfo rigidbody_ci(collider.mass,
                                                              out->motion_state.get(),
                                                              out->shape.get(),
                                                              inertia);
  
  out->rigidbody.reset(new btRigidBody(rigidbody_ci));
  
  world->dynamics_world.addRigidBody(out->rigidbody.get());
}



} // ns