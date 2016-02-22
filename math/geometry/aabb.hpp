#ifndef AABB_INCLUDED_0C5E26F6_BFB1_422A_948B_0E4861927EEE
#define AABB_INCLUDED_0C5E26F6_BFB1_422A_948B_0E4861927EEE


#include "geometry_types.hpp"
#include "../vec/vec3.hpp"
#include "../general/general.hpp"
#include <stddef.h>
#include <assert.h>


namespace math {


// ** Interface ** //

inline void     aabb_from_xyz_array(aabb &out_aabb, const float vertex[], const size_t number_of_floats);


// ** Impl ** //


void
aabb_from_xyz_array(aabb &out_aabb,
                        const float vertex[],
                        const size_t number_of_floats)
{
  assert((number_of_floats % 3) == 0);
  if((number_of_floats % 3) != 0)
  {
    return;
  }

  float max_x(0.f);
  float max_y(0.f);
  float max_z(0.f);
  float min_x(0.f);
  float min_y(0.f);
  float min_z(0.f);

  for(size_t i = 0; i < number_of_floats / 3; ++i)
  {
    size_t index = i * 3;

    max_x = max(vertex[index + 0], max_x);
    min_x = min(vertex[index + 0], min_x);

    max_y = max(vertex[index + 1], max_y);
    min_y = min(vertex[index + 1], min_y);

    max_z = max(vertex[index + 2], max_z);
    min_z = min(vertex[index + 2], min_z);
  }

  out_aabb.max     = vec3_init(max_x, max_y, max_z);
  out_aabb.min     = vec3_init(min_x, min_y, min_z);
  out_aabb.extents = vec3_subtract(out_aabb.max, out_aabb.min);
}


} // ns


#endif // inc guard
