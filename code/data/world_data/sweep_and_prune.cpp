#include "sweep_and_prune.hpp"
#include <assert.h>


namespace World_data {


void
sweep_and_prune_create(Sweep_and_prune *mark_and_sweep, const Core::Entity_id id[], const math::aabb bounds[], const uint32_t number_of_entities, const math::aabb bounds_size)
{
  // Determin size of each bucker.
  const float x_size = (math::vec3_get_x(bounds_size.half_extents) * 2.f) / 64.f;
  const float y_size = (math::vec3_get_y(bounds_size.half_extents) * 2.f) / 64.f;
  const float z_size = (math::vec3_get_z(bounds_size.half_extents) * 2.f) / 64.f;
  
  const float x_start = math::vec3_get_x(bounds_size.min);
  const float y_start = math::vec3_get_y(bounds_size.min);
  const float z_start = math::vec3_get_z(bounds_size.min);
  
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    const math::aabb *curr_bounds = &bounds[i];
  
    const uint32_t x_bucket_start = math::floor(math::vec3_get_x(curr_bounds->min) / 64.f);
    const uint32_t x_bucket_end = math::ceil(math::vec3_get_x(curr_bounds->min) / 64.f);
  }
}


} // ns