#ifndef COLLISION_POINT_INCLUDED_0D78AF7D_13DD_48B4_8A90_0FD2376D21CE
#define COLLISION_POINT_INCLUDED_0D78AF7D_13DD_48B4_8A90_0FD2376D21CE


#include <math/vec/vec_types.hpp>


namespace Physics_transform {


struct Collision_point
{
  float penitration;
  math::vec3 point;
  math::vec3 normal;
};


} // ns


#endif // inc guard