#include "world.hpp"
#include "rigidbody.hpp"
#include "rigidbody_properties.hpp"
#include <algorithm>
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

  world->dynamics_world.stepSimulation(dt, 100  , 1.f / 240.0f);
  //world->dynamics_world.stepSimulation(dt, 1);
}


void
world_add_rigidbodies(World *world,
                      const Rigidbody_properties props[],
                      const std::size_t number_of_rbs_props,
                      Rigidbody destination[],
                      const std::size_t number_of_rbs)
{
  // Param check
  assert(world && props && destination);
  assert(number_of_rbs >= number_of_rbs_props); // Will work but likely you didn't mean todo this.
  
  const std::size_t number_of_rbs_to_process = std::min(number_of_rbs_props, number_of_rbs);

  for(std::size_t i = 0; i < number_of_rbs_to_process; ++i)
  {
    const Rigidbody_properties *prop = &props[i];
    Rigidbody *out_rb = &destination[i];
    
    /*
      Create the collision shape.
    */
    switch(prop->collider_type)
    {
      case(Collider_type::static_plane):
      {
        const btVector3 normal(prop->collider_info.static_plane.normal[0],
                               prop->collider_info.static_plane.normal[1],
                               prop->collider_info.static_plane.normal[2]);
        const btScalar offset(prop->collider_info.static_plane.offset);
        
        out_rb->shape.reset(new btStaticPlaneShape(normal, offset));
        break;
      }
      
      case(Collider_type::cube):
      {
        const btVector3 extents(prop->collider_info.cube.extents[0],
                                prop->collider_info.cube.extents[1],
                                prop->collider_info.cube.extents[2]);

        out_rb->shape.reset(new btBoxShape(extents));
        break;
      }
      
      case(Collider_type::unknown):
      default:
        assert(false); // oh no you didn't.
        return;
    }
    
    /*
      Create the Rigidbody and add it.
    */
    btVector3 inertia(0, 0, 0);
    out_rb->shape->calculateLocalInertia(prop->mass, inertia);
    
    const btRigidBody::btRigidBodyConstructionInfo rigidbody_ci(prop->mass,
                                                                out_rb->motion_state.get(),
                                                                out_rb->shape.get(),
                                                                inertia);
    
    out_rb->rigidbody.reset(new btRigidBody(rigidbody_ci));
    
    world->dynamics_world.addRigidBody(out_rb->rigidbody.get());
  }
}


} // ns