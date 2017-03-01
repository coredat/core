#ifndef PHYISCS_DATA_INCLUDED_F50C8767_EF70_417B_AB25_2D84A48A2416
#define PHYISCS_DATA_INCLUDED_F50C8767_EF70_417B_AB25_2D84A48A2416


#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "utils/debug_renderer.hpp"
#include <utilities/array.hpp>
#include <stdint.h>


namespace Data {
namespace Physics {


struct Rigidbody
{
  uintptr_t rigidbody_ptr;
  uintptr_t shape_ptr;
  uintptr_t motion_state_ptr;
};


struct Trigger
{
  uintptr_t ghost_ptr;
  uintptr_t shape_ptr;
};


struct Physics_data
{
  btBroadphaseInterface               *broadphase       = nullptr;
  btDefaultCollisionConfiguration     *collision_config = nullptr;
  btCollisionDispatcher               *dispatcher       = nullptr;
  btSequentialImpulseConstraintSolver *solver           = nullptr;
  btDiscreteDynamicsWorld             *dynamics_world   = nullptr;
  
  #ifndef NDEBUG
  Bullet_detail::Debug_drawer         debug_drawer;
  #endif
  
  lib::array<uint32_t, 0>             rb_ids;
  lib::array<Rigidbody, 0>            rb_data;
  
  lib::array<uint32_t, 0>             trigger_ids;
  lib::array<Trigger, 0>              trigger_data;
  
  float time_step;
};


} // ns
} // ns


#endif // inc guard
