#include "sweep.hpp"
#include <math/geometry/aabb.hpp>
#include <assert.h>

#define CORE_USE_SCRATCH_ALLOC

#ifdef CORE_USE_SCRATCH_ALLOC
#include <data/memory/memory_data.hpp>
#endif


namespace Physics {
namespace Broadphase {


void
sweep_init(Sweep *sweep,
           const uint32_t number_of_aabbs)
{
  assert(sweep);
  
  sweep->size = number_of_aabbs;

  #ifdef CORE_USE_SCRATCH_ALLOC
  size_t bytes = sizeof(Sweep_axis) * number_of_aabbs;

  sweep->x_axis = new(Memory::scratch_alloc(bytes)) Sweep_axis[number_of_aabbs];
  sweep->y_axis = new(Memory::scratch_alloc(bytes)) Sweep_axis[number_of_aabbs];
  sweep->z_axis = new(Memory::scratch_alloc(bytes)) Sweep_axis[number_of_aabbs];
  #else
  sweep->x_axis = new Sweep_axis[number_of_aabbs];
  sweep->y_axis = new Sweep_axis[number_of_aabbs];
  sweep->z_axis = new Sweep_axis[number_of_aabbs];
  #endif
}


void
sweep_calculate(Sweep *sweep,
                const math::aabb bounding_area[],
                const uint32_t number_of_aabbs)
{
  // Project the aabbs onto the individual axis's.
  for(uint32_t i = 0; i < number_of_aabbs; ++i)
  {
    // Get center point and length
    const math::vec3 origin = math::aabb_get_origin(bounding_area[i]);
    const math::vec3 half_extent = math::aabb_get_half_extents(bounding_area[i]);
   
    sweep->x_axis[i] = Sweep_axis{math::vec3_get_x(origin), math::vec3_get_x(half_extent)};
    sweep->y_axis[i] = Sweep_axis{math::vec3_get_y(origin), math::vec3_get_y(half_extent)};
    sweep->z_axis[i] = Sweep_axis{math::vec3_get_z(origin), math::vec3_get_z(half_extent)};
  }
}


uint32_t
sweep_get_size(const Sweep *sweep)
{
  assert(sweep);

  return sweep->size;
}


void
sweep_free(Sweep *sweep)
{
  assert(sweep);

  #ifndef CORE_USE_SCRATCH_ALLOC
  delete[] sweep->x_axis;
  delete[] sweep->y_axis;
  delete[] sweep->z_axis;
  #endif
}


} // ns
} // ns
