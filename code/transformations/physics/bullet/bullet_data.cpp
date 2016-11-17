#include <transformations/physics/bullet/bullet_data.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <assert.h>


namespace Bullet_data {


void
remove_and_clear(Rigidbody *rb,
                 btDynamicsWorld *world)
{
  // Param check
  assert(rb);
  assert(world);
  
  {
    btRigidBody *bt_rb = reinterpret_cast<btRigidBody*>(rb->rigidbody_ptr);
    assert(bt_rb);
    
    world->removeRigidBody(bt_rb);
    delete bt_rb;
  }
  
  {
    btMotionState *bt_mt = reinterpret_cast<btMotionState*>(rb->motion_state_ptr);
    
    if(bt_mt)
    {
      delete bt_mt;
    }
  }
  
  {
    btCollisionShape *bt_shape = reinterpret_cast<btCollisionShape*>(rb->shape_ptr);
    
    if(bt_shape)
    {
      delete bt_shape;
    }
  }
  
  // Reset to zero.
  memset(rb, 0, sizeof(decltype(*rb)));
}


void
remove_and_clear(Trigger *trigger,
                 btDynamicsWorld *world)
{
  // Param check
  assert(trigger);
  assert(world);

  {
    btGhostObject *bt_gh = reinterpret_cast<btGhostObject*>(trigger->ghost_ptr);
    assert(bt_gh);
    
    world->removeCollisionObject(bt_gh);
    
    delete bt_gh;
  }
  
  {
    btCollisionShape *bt_shape = reinterpret_cast<btCollisionShape*>(trigger->shape_ptr);
    
    if(bt_shape)
    {
      delete bt_shape;
    }
  }
  
  // Reset to zero.
  memset(trigger, 0, sizeof(decltype(*trigger)));
}


void
setup(World *world)
{
  // Param check
  assert(world);

  world->broadphase        = new btDbvtBroadphase();
  world->solver            = new btSequentialImpulseConstraintSolver;
  world->collision_config  = new btDefaultCollisionConfiguration();
  world->dispatcher        = new btCollisionDispatcher(world->collision_config);
  world->dynamics_world    = new btDiscreteDynamicsWorld(world->dispatcher,
                                                         world->broadphase,
                                                         world->solver,
                                                         world->collision_config);
  
  btGImpactCollisionAlgorithm::registerAlgorithm(world->dispatcher);
  world->dynamics_world->setGravity(btVector3(0, -10, 0));
}


void
remove_and_clear(World *world)
{
  // Param check
  assert(world);
  
  if(world->dynamics_world)   { delete world->dynamics_world;   }
  if(world->dispatcher)       { delete world->dispatcher;       }
  if(world->collision_config) { delete world->collision_config; }
  if(world->solver)           { delete world->solver;           }
  if(world->broadphase)       { delete world->broadphase;       }
  
  *world = World();
}



} // ns