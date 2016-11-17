#ifndef BULLET_DATA_INCLUDED_84ABBDE9_4582_4635_89BC_3F40B09E03F6
#define BULLET_DATA_INCLUDED_84ABBDE9_4582_4635_89BC_3F40B09E03F6


#include <stdint.h>


class btDynamicsWorld;


namespace Bullet_data {


struct Rigidbody
{
  uintptr_t rigidbody_ptr    = 0;
  uintptr_t shape_ptr        = 0;
  uintptr_t motion_state_ptr = 0;
};


struct Trigger
{
  uintptr_t ghost_ptr = 0;
  uintptr_t shape_ptr = 0;
};


void
remove_and_clear(Rigidbody *rb, btDynamicsWorld *world);


void
remove_and_clear(Trigger *trigger, btDynamicsWorld *world);


} // ns


#endif // inc guard