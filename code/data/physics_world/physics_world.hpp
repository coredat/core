#ifndef PHYSICS_WORLD_INCLUDED_2B7C3AF5_17F9_49E4_8603_7919D93A39F4
#define PHYSICS_WORLD_INCLUDED_2B7C3AF5_17F9_49E4_8603_7919D93A39F4


#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <data/entity/entity_data.hpp>
#include <data/physics_world/data_motion_state.hpp>


namespace Data {


struct Physics_world
{
  btDbvtBroadphase                       broadphase;
  btDefaultCollisionConfiguration        collision_config;
  btCollisionDispatcher                  dispatcher = {&collision_config};
  btSequentialImpulseConstraintSolver    solver;
  btDiscreteDynamicsWorld                dynamics_world = {&dispatcher, &broadphase, &solver, &collision_config};
//  Bullet::Detail::Debug_draw             m_debug_draw;
};





void physics_world_init(Physics_world *world);
void physics_world_step(Physics_world *world, const float dt);
void physics_add_rigidbody(Physics_world *world, const Entity_id entity, const Data::Entity_data *data);


} // ns


#endif // inc guard