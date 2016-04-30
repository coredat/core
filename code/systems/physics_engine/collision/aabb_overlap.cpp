#include "aabb_overlap.hpp"
#include "collision_pairs.hpp"
#include <math/geometry/aabb.hpp>


namespace Physics {
namespace Collision {


void
aabb_calculate_overlaps_pairs(Pairs *out_pairs,
                              const math::aabb aabbs[],
                              const uint32_t number_of_transforms)
{
  for(uint32_t i = 0; i < number_of_transforms; ++i)
  {
    const math::aabb *to_test = &aabbs[i];
  
    for(uint32_t j = 0; j < number_of_transforms; ++j)
    {
      // Don't self test.
      if(i == j)
      {
        continue;
      }
    
      const math::aabb *test_against = &aabbs[j];
      
      if(math::aabb_intersection_test(*to_test, *test_against))
      {
        pairs_add(out_pairs, Pair{i, j});
      }
    }
  }
}


} // ns
} // ns