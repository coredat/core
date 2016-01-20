#ifndef COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F
#define COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F


#include <stdint.h>


namespace Physics {


namespace Axis {


enum ENUM
{
  x = 1 << 0,
  y = 1 << 1,
  z = 1 << 2,
};


} // ns





// TODO: Make this struct smaller.
// Can merge axis
// Can reduce collider_type
// Might be able to remove a float from collider details.
struct Rigidbody_properties
{
  float         mass          = 1;
  bool          make_rb       = true;  
  uint32_t      move_axis     = Axis::x | Axis::y | Axis::z;
  uint32_t      rotation_axis = Axis::x | Axis::y | Axis::z;
};


} // ns


#endif // inc guard