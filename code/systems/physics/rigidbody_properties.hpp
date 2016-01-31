#ifndef COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F
#define COLLIDER_INCLUDED_991C680D_ABCA_4E7A_A742_8E324B855F1F


#include <systems/entity/entity_id.hpp>
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
struct Rigidbody_properties
{
  float         mass          = 1;
  uint32_t      move_axis     = Axis::x | Axis::y | Axis::z;
  uint32_t      rotation_axis = Axis::x | Axis::y | Axis::z;
  Core::Entity_id id;
};


} // ns


#endif // inc guard