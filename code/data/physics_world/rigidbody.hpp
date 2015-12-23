#ifndef RIGIDBODY_INCLUDED_
#define RIGIDBODY_INCLUDED_

#include <btBulletDynamicsCommon.h>
#include "data_motion_state.hpp"
#include <memory>

struct Rigidbody
{
  std::unique_ptr<Data_detail::Motion_state>  motion_state;
  std::unique_ptr<btRigidBody>                rigidbody;
};


#endif // inc guard