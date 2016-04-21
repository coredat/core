#include "physics_engine.hpp"
#include <iostream>


namespace Physics_engine {

void
get_collisions(
  const Core::Entity_id ids[],
  const math::transform transforms[],
  const math::aabb aabbs[],
  const uint32_t number_of_entities_to_test,
  Collision_pair out_ids[],
  const uint32_t size_of_out_buffer,
  uint32_t *number_of_collisions
)
{
  (*number_of_collisions) = 0;

  for(uint32_t i = 0; i < number_of_entities_to_test; ++i)
  {
    math::aabb aabb_a = aabbs[i];
//    math::aabb_scale(aabb_a, transforms[i].scale);
    aabb_a.origin = transforms[i].position;
  
    for(uint32_t j = 0; j < number_of_entities_to_test; ++j)
    {
      if(i != j)
      {
        math::aabb aabb_b = aabbs[j];
//        math::aabb_scale(aabb_b, transforms[j].scale);
        
        aabb_b.origin = transforms[j].position;
        
        if(math::aabb_intersection_test(aabb_a, aabb_b))
        {
          out_ids[(*number_of_collisions)] = Collision_pair{ids[i], ids[j]};
          (*number_of_collisions) += 1;
          
          if(*number_of_collisions > size_of_out_buffer)
          {
            return;
          }
        }
      }
    }
  }
}


//void
//get_collisions(
//  const Core::Entity_id ids[],
//  const math::transform transforms[],
//  const math::aabb aabbs[],
//  const uint32_t number_of_entities_to_test,
//  Collision_pair out_ids[],
//  const uint32_t size_of_out_buffer,
//  uint32_t *number_of_collisions
//)
//{
//  (*number_of_collisions) = 0;
//
//  for(uint32_t i = 0; i < number_of_entities_to_test; ++i)
//  {
//    math::aabb aabb_a = aabbs[i];
//    math::aabb_scale(aabb_a, transforms[i].scale);
//    aabb_a.origin = transforms[i].position;
//  
//    for(uint32_t j = 0; j < number_of_entities_to_test; ++j)
//    {
//      if(i != j)
//      {    
//        math::aabb aabb_b = aabbs[j];
//        math::aabb_scale(aabb_b, transforms[j].scale);
//        
//        aabb_b.origin = transforms[j].position;
//        
//        if(math::aabb_intersection_test(aabb_a, aabb_b))
//        {
//          out_ids[(*number_of_collisions)] = Collision_pair{ids[i], ids[j]};
//          (*number_of_collisions) += 1;
//          
//          if(*number_of_collisions > size_of_out_buffer)
//          {
//            return;
//          }
//        }
//      }
//    }
//  }
//}


} // ns