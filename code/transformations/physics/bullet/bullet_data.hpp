#ifndef BULLET_DATA_INCLUDED_84ABBDE9_4582_4635_89BC_3F40B09E03F6
#define BULLET_DATA_INCLUDED_84ABBDE9_4582_4635_89BC_3F40B09E03F6


#include <stdint.h>


namespace Bullet_data {


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


} // ns


#endif // inc guard