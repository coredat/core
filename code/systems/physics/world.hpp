#ifndef WORLD_INCLUDED_4284F567_42B4_4037_A058_D4382B9FE5D8
#define WORLD_INCLUDED_4284F567_42B4_4037_A058_D4382B9FE5D8


#include "physics_fwd.hpp"
#include "debug_renderer.hpp"
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <memory>


namespace Physics {


/*!
  The physical world world.
*/
struct World
{
  World() {}

  btDbvtBroadphase                       broadphase;
  btDefaultCollisionConfiguration        collision_config;
  btCollisionDispatcher                  dispatcher = {&collision_config};
  btSequentialImpulseConstraintSolver    solver;
  btDiscreteDynamicsWorld                dynamics_world = {&dispatcher, &broadphase, &solver, &collision_config};
  std::unique_ptr<btIDebugDraw>          debug_draw;
};


/*!
  Initializes the physics world.
  \param world a valid instance of the physical world.
*/
void
world_init(World *world);

/*!
  Run the physics simulation.
  \param world a valid instance of the physical world.
  \param dt delta time step.
*/
void
world_step(World *world, const float dt);

/*!
  Run the physics simulation.
  \param world a valid instance of the physical world.
  \param Rigidbody_properties An array of rigidbody properties.
  \param number_of_rb_props Number of input properties in the array.
  \param destination The array of ouptut Rigidbodies that are created from the properties.
  \param number_of_rbs Number of elements available in the output.
*/
void
world_add_rigidbodies(World *world,
                      const Rigidbody_properties rb_properties[],
                      const std::size_t number_of_rb_props,
                      Rigidbody destination[],
                      const std::size_t number_of_rbs);


} // ns


#endif