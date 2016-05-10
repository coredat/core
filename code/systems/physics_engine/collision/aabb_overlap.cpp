#include "aabb_overlap.hpp"
#include "collision_pairs.hpp"
#include "axis_collidable.hpp"
#include <math/geometry/aabb.hpp>
#include <iostream>

namespace Physics {
namespace Collision {


void
aabb_calculate_overlaps_pairs(Pairs *out_pairs,
                              const Axis_collidable axis_colliders[],
                              const uint32_t number_of_colliders)
{
  uint32_t test_count = 0;
  uint32_t test_pass = 0;
  
  for(uint32_t i = 0; i < number_of_colliders; ++i)
  {
    const Axis_collidable *to_test = &axis_colliders[i];
    
    for(uint32_t j = 0; j < number_of_colliders; ++j)
    {
      // Don't self test.
      if(i == j)
      {
        continue;
      }
      
      const Axis_collidable *test_against = &axis_colliders[j];

      if(test_against->mask.id_and_mask[0] & to_test->mask.id_and_mask[1])
      {
        test_count++;
        if(math::aabb_intersection_test(to_test->bounding_box, test_against->bounding_box))
        {
          test_pass++;
          pairs_add(out_pairs, Pair{i, j});
        }
      }
    }
  }
  
  std::cout << "AABB Tested/Passed: " << test_count << "/" << test_pass << std::endl;
}


} // ns
} // ns