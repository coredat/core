#include "../physics.hpp"
#include "../config_physics.hpp"
#include "physics_data.hpp"
#include "utils/bullet_math_extensions.hpp"
#include <utilities/assert.hpp>


// ------------------------------------------------------[ Physics Lifetime ]--


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
    
    #ifndef NDEBUG
    world->dynamics_world->setDebugDrawer(&world->debug_drawer);
    #endif

    world->time_step = config.time_step;    
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
