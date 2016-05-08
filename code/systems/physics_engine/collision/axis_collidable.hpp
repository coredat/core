#ifndef AXIS_COLLIDABLE_INCLUDED_53FBA3EA_C7C2_4E2C_8BA2_A644BF8D08D1
#define AXIS_COLLIDABLE_INCLUDED_53FBA3EA_C7C2_4E2C_8BA2_A644BF8D08D1


#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


namespace Physics {
namespace Collision {


struct Axis_collidable
{
  union Mask
  {
    uint64_t      bits;
    uint32_t      id_and_mask[2];
  } mask;

  math::aabb    bounding_box;
};


} // ns
} // ns


#endif // inc guard