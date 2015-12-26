#ifndef WORLD_INCLUDED_4284F567_42B4_4037_A058_D4382B9FE5D8
#define WORLD_INCLUDED_4284F567_42B4_4037_A058_D4382B9FE5D8


#include "physics_fwd.hpp"
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>


namespace Physics {


struct World
{
  btDbvtBroadphase                       broadphase;
  btDefaultCollisionConfiguration        collision_config;
  btCollisionDispatcher                  dispatcher = {&collision_config};
  btSequentialImpulseConstraintSolver    solver;
  btDiscreteDynamicsWorld                dynamics_world = {&dispatcher, &broadphase, &solver, &collision_config};
//  Bullet::Detail::Debug_draw             m_debug_draw;
};


void world_init(World *world);
void world_step(World *world, const float dt);
void world_add_rigidbodies(World *world,
                           const Rigidbody_properties rb_properties[],
                           const std::size_t number_of_rb_props,
                           Rigidbody destination[],
                           const std::size_t number_of_rbs);


} // ns


#endif