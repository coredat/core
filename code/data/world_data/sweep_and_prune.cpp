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
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    const math::aabb *curr_bounds = &bounds[i];
    
    const float number_of_buckets = 16.f;
    
    const float x_bucket_scale = (math::vec3_get_x(bounds_size.half_extents) * 2.f) / number_of_buckets;
    const uint32_t x_bucket_start = math::to_uint(math::floor(math::vec3_get_x(curr_bounds->min) / x_bucket_scale));
    const uint32_t x_bucket_end   = math::to_uint(math::floor(math::vec3_get_x(curr_bounds->max) / x_bucket_scale));
    
    const float y_bucket_scale = (math::vec3_get_y(bounds_size.half_extents) * 2.f) / number_of_buckets;
    const uint32_t y_bucket_start = math::to_uint(math::floor(math::vec3_get_y(curr_bounds->min) / y_bucket_scale));
    const uint32_t y_bucket_end   = math::to_uint(math::floor(math::vec3_get_y(curr_bounds->max) / y_bucket_scale));

    const float z_bucket_scale = (math::vec3_get_z(bounds_size.half_extents) * 2.f) / number_of_buckets;
    const uint32_t z_bucket_start = math::to_uint(math::floor(math::vec3_get_z(curr_bounds->min) / z_bucket_scale));
    const uint32_t z_bucket_end   = math::to_uint(math::floor(math::vec3_get_z(curr_bounds->max) / z_bucket_scale));
    
    // Push the aabb into all the buckets it belongs to.
    for(uint32_t x = x_bucket_start; x < x_bucket_end; ++x) {
      for(uint32_t y = y_bucket_start; y < y_bucket_end; ++y) {
        for(uint32_t z = z_bucket_start; z < z_bucket_end; ++z)
        {
          const uint32_t no_buckets = math::to_uint(number_of_buckets);
          const uint32_t index = (x + (y * no_buckets) + (z * no_buckets * no_buckets));
          Bucket *bucket = &mark_and_sweep->bucket[index];
          const uint32_t curr_size = bucket->size;
          
          bucket->entity_bounds[curr_size] = bounds[i];
          bucket->id[curr_size] = id[i];
          
          ++(bucket->size);
        }
      }
    }
  }
}


} // ns