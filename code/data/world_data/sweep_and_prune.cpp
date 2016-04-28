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
//  mark_and_sweep->bucket.resize(64 * 64 * 64);

  static std::vector<Core::Entity_id> ids;
  // position and half extent
  static std::vector<std::pair<float, float>> x_pairs;
  static std::vector<std::pair<float, float>> y_pairs;
  static std::vector<std::pair<float, float>> z_pairs;
  
  ids.clear();
  x_pairs.clear();
  y_pairs.clear();
  z_pairs.clear();

  // Sweep
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    const math::aabb *curr_bound = &bounds[i];
    
    math::vec3 center = math::aabb_get_origin(*curr_bound);
    math::vec3 half_extent = math::aabb_get_half_extents(*curr_bound);
    
    ids.push_back(id[i]);
    x_pairs.push_back(std::pair<float, float>(math::vec3_get_x(center), math::vec3_get_x(half_extent)));
    y_pairs.push_back(std::pair<float, float>(math::vec3_get_y(center), math::vec3_get_y(half_extent)));
    z_pairs.push_back(std::pair<float, float>(math::vec3_get_z(center), math::vec3_get_z(half_extent)));
  }
  
  // Prune x
  auto prune = [&](auto axis)
  {
    uint32_t i = 0;
    
    while(i < axis.size())
    {
      uint32_t j = 0;
      std::pair<float, float> self = axis[i];
      
      while(j < axis.size())
      {
        std::pair<float, float> test = axis[j];
        
        // Do these overlap? if so break they might be colliding.
        {
          const float distance = math::abs(test.first - self.first);
          const float combined_half_extents = test.second + self.second;
          
          // If we have a potential collision stop.
          if(distance < combined_half_extents && (j != i))
          {
            break;
          }
          // Keep looking.
          else if(j < axis.size() - 1)
          {
            ++j;
            continue;
          }
          // No collision on this axis, means no valid collision
          else
          {
            ids.erase(std::begin(ids) + i - 1);
            x_pairs.erase(std::begin(x_pairs) + i - 1);
            y_pairs.erase(std::begin(y_pairs) + i - 1);
            z_pairs.erase(std::begin(z_pairs) + i - 1);
            
            ++i;
          }
        }
      }
      
      ++i;
    }
  };
  
  prune(x_pairs);
  prune(y_pairs);
  prune(z_pairs);
  
}


} // ns