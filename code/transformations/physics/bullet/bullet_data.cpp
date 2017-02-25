#include <transformations/physics/bullet/bullet_data.hpp>
#include <transformations/physics/bullet/bullet_math_extensions.hpp>
#include <core/color/color.hpp>
#include <core/debug/line_renderer.hpp>
#include <core/debug/point_renderer.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <LinearMath/btIDebugDraw.h>
#include <utilities/assert.hpp>


namespace Bullet_data {


void
remove_and_clear(Rigidbody *rb,
                 btDynamicsWorld *world)
{
  // -- Param Check -- //
  UTIL_ASSERT(rb);
  UTIL_ASSERT(world);
  
  {
    btRigidBody *bt_rb(
      reinterpret_cast<btRigidBody*>(rb->rigidbody_ptr)
    );
    UTIL_ASSERT(bt_rb);
    
    world->removeRigidBody(bt_rb);
    delete bt_rb;
  }
  
  {
    btMotionState *bt_mt(
      reinterpret_cast<btMotionState*>(rb->motion_state_ptr)
    );
    
    if(bt_mt)
    {
      delete bt_mt;
    }
  }
  
  {
    btCollisionShape *bt_shape(
      reinterpret_cast<btCollisionShape*>(rb->shape_ptr)
    );
    
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
  // -- Param Check -- //
  UTIL_ASSERT(trigger);
  UTIL_ASSERT(world);

  {
    btGhostObject *bt_gh(
      reinterpret_cast<btGhostObject*>(trigger->ghost_ptr)
    );
    UTIL_ASSERT(bt_gh);
    
    world->removeCollisionObject(bt_gh);
    
    delete bt_gh;
  }
  
  {
    btCollisionShape *bt_shape(
      reinterpret_cast<btCollisionShape*>(trigger->shape_ptr)
    );
    
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
  struct Debug_drawer : public btIDebugDraw
  {
    void
    drawLine(const btVector3& from,
             const btVector3& to,
             const btVector3& color)
    {
      Core::Debug::debug_line(math::vec3_from_bt(from), math::vec3_from_bt(to), 0XFF0000FF);
    }
    
    void
    drawContactPoint(const btVector3& PointOnB,
                     const btVector3& normalOnB,
                     btScalar distance,
                     int lifeTime,
                     const btVector3& color)
    {
      Core::Debug::draw_axis_cross(math::vec3_from_bt(PointOnB));
    }

    void
    reportErrorWarning(const char* warningString)
    {
    }

    void
    draw3dText(const btVector3& location,
               const char* textString)
    {
    }
    
    void
    setDebugMode(int mode)
    {
      debug_mode = mode;
    }
    
    int
    getDebugMode() const
    {
      return debug_mode;
    }
    
      int debug_mode = 0
             | btIDebugDraw::DBG_DrawContactPoints
//             | btIDebugDraw::DBG_DrawAabb
             | btIDebugDraw::DBG_DrawWireframe
             ;
  };
  
  static Debug_drawer debug_drawer;

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
  world->dynamics_world->setDebugDrawer(&debug_drawer);
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
