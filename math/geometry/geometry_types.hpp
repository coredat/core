#ifndef GEOMETRY_TYPES_INCLUDED_5C543B7A_4C9E_45FE_91BD_F072716934F0
#define GEOMETRY_TYPES_INCLUDED_5C543B7A_4C9E_45FE_91BD_F072716934F0


#include "../vec/vec3.hpp"


namespace math {


struct aabb
{
  math::vec3 max          = math::vec3_zero();
  math::vec3 min          = math::vec3_zero();
  math::vec3 half_extents = math::vec3_zero();
  math::vec3 origin       = math::vec3_zero();
};


struct ray
{
  math::vec3 start      = math::vec3_zero();
  math::vec3 end        = math::vec3_zero();
};


} // ns


#endif // inc guard
