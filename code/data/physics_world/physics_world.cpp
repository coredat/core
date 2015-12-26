#include "physics_world.hpp"
#include "rigidbody.hpp"
#include "rigidbody_properties.hpp"
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
world_add_rigidbody(World *world, const Rigidbody_properties props, Rigidbody *out)
{
  assert(world && out);
  
  switch(props.collider_type)
  {
    case(Collider_type::static_plane):
    {
      const btVector3 normal(props.collider_info.static_plane.normal[0],
                             props.collider_info.static_plane.normal[1],
                             props.collider_info.static_plane.normal[2]);
      const btScalar offset(props.collider_info.static_plane.offset);
      
      out->shape.reset(new btStaticPlaneShape(normal, offset));
      break;
    }
    
    case(Collider_type::cube):
    {
      const btVector3 extents(props.collider_info.cube.extents[0],
                              props.collider_info.cube.extents[1],
                              props.collider_info.cube.extents[2]);

      out->shape.reset(new btBoxShape(extents));
      break;
    }
    
    case(Collider_type::unknown):
    default:
      assert(false); // oh no you didn't.
      return;
  }
  
  btVector3 inertia(0, 0, 0);
  out->shape->calculateLocalInertia(props.mass, inertia);
  
  //rb.motion_state.reset(new Data_detail::Motion_state(entity, data));
  const btRigidBody::btRigidBodyConstructionInfo rigidbody_ci(props.mass,
                                                              out->motion_state.get(),
                                                              out->shape.get(),
                                                              inertia);
  
  out->rigidbody.reset(new btRigidBody(rigidbody_ci));
  
  world->dynamics_world.addRigidBody(out->rigidbody.get());
}


void
world_add_rigidbodies(World *world, const Rigidbody_properties props[], const std::size_t number_of_rbs, Rigidbody destination[], const std::size_t stride)
{
  for(std::size_t i = 0; i < number_of_rbs; ++i)
  {
    const Rigidbody_properties *prop = &props[i];
    Rigidbody *out_rb = &destination[i];
    
    switch(props->collider_type)
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
    
    btVector3 inertia(0, 0, 0);
    out_rb->shape->calculateLocalInertia(props->mass, inertia);
    
    const btRigidBody::btRigidBodyConstructionInfo rigidbody_ci(prop->mass,
                                                                out_rb->motion_state.get(),
                                                                out_rb->shape.get(),
                                                                inertia);
    
    out_rb->rigidbody.reset(new btRigidBody(rigidbody_ci));
    
    world->dynamics_world.addRigidBody(out_rb->rigidbody.get());
  }
}


} // ns