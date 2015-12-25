#include "physics_world.hpp"
#include <data/entity/entity_data.hpp>
#include <assert.h>


namespace Physics {


void
world_init(World *world)
{
  assert(world);
  
  world->dynamics_world.setGravity(btVector3(0, -10, 0));
  //world->dynamics_world.setDebugDrawer(&debug_draw);
}


void
world_step(World *world, const float dt)
{
  assert(world);

  world->dynamics_world.stepSimulation(dt / 1, 10);
}


void
world_add_rigidbody(World *world, const Collider_detail collider, Rigidbody *out)
{
  assert(world && out);
  
  switch(collider.type)
  {
    case(Collider_type::static_plane):
    {
      const btVector3 normal(collider.info.static_plane.normal[0],
                             collider.info.static_plane.normal[1],
                             collider.info.static_plane.normal[2]);
      const btScalar offset(collider.info.static_plane.offset);
      
      out->shape.reset(new btStaticPlaneShape(normal, offset));
      break;
    }
    
    case(Collider_type::cube):
    {
      const btVector3 extents(collider.info.cube.extents[0],
                              collider.info.cube.extents[1],
                              collider.info.cube.extents[2]);

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


void
world_add_rigidbodies(World *world, const Collider_detail colliders[], const std::size_t number_of_colliders, Rigidbody *destination)
{
  
}


} // ns