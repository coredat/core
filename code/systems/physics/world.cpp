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
  
  //world->debug_draw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
  
  world->debug_draw.reset(new Debug_renderer());
  world->dynamics_world.setDebugDrawer(world->debug_draw.get());
}


void
world_step(World *world, const float dt)
{
  assert(world);

  world->dynamics_world.stepSimulation(dt, 100  , 1.f / 240.0f);
  //world->dynamics_world.stepSimulation(dt, 1);
  
  world->dynamics_world.debugDrawWorld();
}


void
world_add_rigidbodies(World *world,
                      const Rigidbody_properties props[],
                      const std::size_t number_of_rbs_props,
                      Rigidbody* destination[],
                      const std::size_t number_of_rbs)
{
  // Param check
  assert(world && props && destination);
  assert(number_of_rbs >= number_of_rbs_props); // Will work but likely you didn't mean todo this.
  
  const std::size_t number_of_rbs_to_process = std::min(number_of_rbs_props, number_of_rbs);

  for(std::size_t i = 0; i < number_of_rbs_to_process; ++i)
  {
    const Rigidbody_properties *prop = &props[i];
    Rigidbody *out_rb = destination[i];
    
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
      
      case(Collider_type::capsule):
      {
        const btScalar radius(prop->collider_info.capsule.radius);
        const btScalar height(prop->collider_info.capsule.height);
        
        out_rb->shape.reset(new btCapsuleShape(radius, height));
        break;
      }
      
      case(Collider_type::none):
        continue; // not a rigidbody
      
      default:
        assert(false); // unknown collider type.
        return;
    }
    
    /*
      Check if rb is already in the scene.
      Currently we shall just pull it from the world.
      However this might need to change to a nicer update.
    */
    if(out_rb->rigidbody)
    {
      world->dynamics_world.removeRigidBody(out_rb->rigidbody.get());
      out_rb->rigidbody.reset();
    }
    
    /*
      Create a compound collider if none exists
    */
    if(!out_rb->compound_shape)
    {
      out_rb->compound_shape.reset(new btCompoundShape());
    }
    
    /*
      Create the Rigidbody and add it.
    */
    {
      assert(out_rb->shape);
    
      btVector3 inertia(0, 0, 0);
      out_rb->shape->calculateLocalInertia(prop->mass, inertia);
      
      btTransform child_transform ;
      child_transform.setIdentity(); // Warning do not remove this. It seems to create an AABB overflow in Bullet.
 
      out_rb->compound_shape->addChildShape(child_transform, out_rb->shape.get());
      out_rb->compound_shape->calculateLocalInertia(prop->mass, inertia);
      
      const btRigidBody::btRigidBodyConstructionInfo rigidbody_ci(prop->mass,
                                                                  out_rb->motion_state.get(),
                                                                  out_rb->compound_shape.get(),
                                                                  inertia);
      
      out_rb->rigidbody.reset(new btRigidBody(rigidbody_ci));
    }
    
    /*
      Constrain axis
    */
    {
      assert(out_rb->rigidbody);
      btRigidBody *rigidbody = out_rb->rigidbody.get();
      
      const uint32_t movement_axis = prop->move_axis;
      const uint32_t rotation_axis = prop->rotation_axis;
    
      const btVector3 axis_movement((btScalar)(movement_axis >> 0 & 1),
                                    (btScalar)(movement_axis >> 1 & 1),
                                    (btScalar)(movement_axis >> 2 & 1));
      
      const btVector3 axis_rotation((btScalar)(rotation_axis >> 0 & 1),
                                    (btScalar)(rotation_axis >> 1 & 1),
                                    (btScalar)(rotation_axis >> 2 & 1));

      rigidbody->setLinearFactor(axis_movement);
      rigidbody->setAngularFactor(axis_rotation);
    }
    
    /*
      General settings
    */
    {
      assert(out_rb->rigidbody);
      btRigidBody *rigidbody = out_rb->rigidbody.get();
    
      rigidbody->setDamping(0.9f, 0.9f);
      rigidbody->setFriction(0.7f);
      rigidbody->setRollingFriction(0.4f);
      rigidbody->setRestitution(0.f);
    }
    
    /*
      Add Bullet's rigidbody to the world
    */
    {
      assert(out_rb->rigidbody && world);
      world->dynamics_world.addRigidBody(out_rb->rigidbody.get());
    }
  }
}


} // ns