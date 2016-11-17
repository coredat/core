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



} // ns