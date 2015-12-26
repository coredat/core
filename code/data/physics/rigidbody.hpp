#ifndef RIGIDBODY_INCLUDED_52FD6DE0_2B99_45F3_93DB_E55B1FA97275
#define RIGIDBODY_INCLUDED_52FD6DE0_2B99_45F3_93DB_E55B1FA97275


#include "motion_state.hpp"
#include <btBulletDynamicsCommon.h>
#include <memory>


namespace Physics {


struct Rigidbody
{
  std::unique_ptr<Motion_state>       motion_state;
  std::unique_ptr<btRigidBody>        rigidbody;
  std::unique_ptr<btCollisionShape>   shape;
};


} // ns


#endif // inc guard