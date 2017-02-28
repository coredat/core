#include "../physics.hpp"
#include "../config_physics.hpp"
#include "physics_data.hpp"
#include "utils/bullet_math_extensions.hpp"
#include <utilities/assert.hpp>


// ------------------------------------------------------[ Physics Lifetime ]--


bool Data::Physics::g_display_debug_info = true;


namespace Data {
namespace Physics {


Physics_data*
initialize(
  const Physics_config config,
  const lib::malloc_fn malloc_fn,
  const lib::realloc_fn realloc_fn,
  const lib::free_fn free_fn)
{
  // -- Param Check -- //
  LIB_ASSERT(malloc_fn);
  LIB_ASSERT(realloc_fn);
  LIB_ASSERT(free_fn);
  
  void *data = malloc_fn(sizeof(Physics_data));
  LIB_ASSERT(data);
  
  Physics_data *world = new(data) Physics_data();
  LIB_ASSERT(world);
  
  // -- Setup the Phys World -- //
  {
//    struct Debug_drawer : public btIDebugDraw
//    {
//      void
//      drawLine(const btVector3& from,
//               const btVector3& to,
//               const btVector3& color)
//      {
//        Core::Debug::debug_line(math::vec3_from_bt(from), math::vec3_from_bt(to), 0XFF0000FF);
//      }
//      
//      void
//      drawContactPoint(const btVector3& PointOnB,
//                       const btVector3& normalOnB,
//                       btScalar distance,
//                       int lifeTime,
//                       const btVector3& color)
//      {
//        Core::Debug::draw_axis_cross(math::vec3_from_bt(PointOnB));
//      }
//
//      void
//      reportErrorWarning(const char* warningString)
//      {
//      }
//
//      void
//      draw3dText(const btVector3& location,
//                 const char* textString)
//      {
//      }
//      
//      void
//      setDebugMode(int mode)
//      {
//        debug_mode = mode;
//      }
//      
//      int
//      getDebugMode() const
//      {
//        return debug_mode;
//      }
//      
//        int debug_mode = 0
//               | btIDebugDraw::DBG_DrawContactPoints
//  //             | btIDebugDraw::DBG_DrawAabb
//               | btIDebugDraw::DBG_DrawWireframe
//               ;
//    };
//    
//    static Debug_drawer debug_drawer;

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
    world->dynamics_world->setGravity(btVector3(0.f, -10.f, 0.f));
//    world->dynamics_world->setDebugDrawer(&debug_drawer);

    world->time_step = config.time_step;
    
    // Set the default debug view.
    #ifndef NDEBUG
    Data::Physics::g_display_debug_info = true;
    #else
    Data::Physics::g_display_debug_info = false;
    #endif
  }
  
  // -- Setup data containers -- //
  {
    world->rb_ids.reserve(config.inital_rb_count);
    world->rb_data.reserve(config.inital_rb_count);
    
    world->trigger_ids.reserve(config.inital_trigger_count);
    world->trigger_data.reserve(config.inital_trigger_count);
  }

  return world;
}
  
  
bool
destroy(Physics_data *phys)
{
  return false;
}


} // ns
} // ns
