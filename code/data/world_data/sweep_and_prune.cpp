#include "sweep_and_prune.hpp"
#include <assert.h>


namespace World_data {


void
sweep_and_prune_create(Sweep_and_prune *mark_and_sweep,
                       const Core::Entity_id id[],
                       const math::aabb bounds[],
                       const uint32_t number_of_entities,
                       const math::aabb bounds_size)
{
  mark_and_sweep->bucket.resize(64 * 64 * 64);

  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    const math::aabb *curr_bounds = &bounds[i];
    const float number_of_buckets = 64.f;
    
    const math::vec3 buckets = math::vec3_init(64.f, 64.f, 64.f);
    const math::vec3 extent = math::vec3_scale(math::aabb_get_half_extents(bounds_size), 2.f);
    const math::vec3 scale = math::vec3_divide(buckets, extent);
    
    const math::vec3 min = math::vec3_add(curr_bounds->min, math::aabb_get_origin(*curr_bounds));
    const math::vec3 max = math::vec3_add(curr_bounds->max, math::aabb_get_origin(*curr_bounds));
    
    const math::vec3 offset_origin_min = math::vec3_add(math::aabb_get_origin(bounds_size), min);
    const math::vec3 offset_origin_max = math::vec3_add(math::aabb_get_origin(bounds_size), max);
    
    const math::vec3 offset_min = math::vec3_multiply(math::vec3_add(offset_origin_min, math::aabb_get_half_extents(bounds_size)), scale);
    const math::vec3 offset_max = math::vec3_multiply(math::vec3_add(offset_origin_max, math::aabb_get_half_extents(bounds_size)), scale);
    
    // Get ranges.
    const uint32_t get_x_start = math::vec3_get_x(offset_min);
    const uint32_t get_x_end = math::vec3_get_x(offset_max);
    
    const uint32_t get_y_start = math::vec3_get_y(offset_min);
    const uint32_t get_y_end = math::vec3_get_y(offset_max);
    
    const uint32_t get_z_start = math::vec3_get_z(offset_min);
    const uint32_t get_z_end = math::vec3_get_z(offset_max);
    
    for(uint32_t x = get_x_start; x < get_x_end; ++x) {
      for(uint32_t y = get_y_start; y < get_y_end; ++y) {
        for(uint32_t z = get_z_start; z < get_z_end; ++z)
        {
          const uint32_t index = x + (y * 64) + (z * 64 * 64);
          
          mark_and_sweep->bucket[index].entity_bounds.push_back(bounds[i]);
          mark_and_sweep->bucket[index].ids.push_back(id[i]);
        }
      }
    }
    
  }
}


} // ns