#include <transformations/physics/overlapping_aabb.hpp>
#include <data/global_data/memory_data.hpp>
#include <math/geometry/aabb.hpp>
#include <math/transform/transform.hpp>
#include <systems/physics_engine/broadphase/sweep.hpp>
#include <systems/physics_engine/broadphase/prune.hpp>
#include <systems/physics_engine/collision/axis_collidable.hpp>
#include <systems/physics_engine/collision/collision_pairs.hpp>
#include <systems/physics_engine/collision/aabb_overlap.hpp>
#include <utilities/generic_id.hpp>
#include <utilities/logging.hpp>


namespace Transformation {


void
get_overlapping(const util::generic_id ids[],
                const uint64_t collision_ids[],
                const math::aabb colliders[],
                const math::transform transforms[],
                const size_t number_of_bounds,
                Physics::Collision::Axis_collidable *out_axis_collidables[],
                util::generic_id *out_ids[],
                size_t *out_size)
{
  LOG_TODO_ONCE("This needs a refactor");
  
  math::aabb *bounds = SCRATCH_ALIGNED_ALLOC(math::aabb, number_of_bounds);
  
  Transformation::calculate_positional_aabb(colliders,
                                            transforms,
                                            bounds,
                                            number_of_bounds);
  
  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, number_of_bounds);
  
  // is bounds correct here we seem to already have bounds.
  Physics::Broadphase::sweep_calculate(&sweep, bounds, number_of_bounds);
  
  Physics::Broadphase::Prune prune;
  Physics::Broadphase::prune_init(&prune, &sweep);
  Physics::Broadphase::prune_calculate(&prune, &sweep);

  // Prune out
  static std::vector<util::generic_id> id;
  static std::vector<Physics::Collision::Axis_collidable> boxes;
  
  id.clear();
  boxes.clear();
  
  uint32_t prune_stack = 0;
  
  for(uint32_t i = 0; i < number_of_bounds; ++i)
  {
    if(prune_stack < prune.size && i == prune.ids[prune_stack])
    {
      ++prune_stack;
            
      continue;
    }
    
    id.push_back(ids[i]);
    
    math::aabb box_copy(colliders[i]);
    uint64_t collision_mask(collision_ids[i]);
    math::aabb_scale(box_copy, transforms[i].scale);
    math::aabb_set_origin(box_copy, transforms[i].position);
    
    boxes.push_back(Physics::Collision::Axis_collidable{{collision_mask}, box_copy});
  }

  *out_axis_collidables = boxes.data();
  *out_ids = id.data();
  *out_size = boxes.size();
  
  Physics::Broadphase::sweep_free(&sweep);
  Physics::Broadphase::prune_free(&prune);
}


void
calculate_positional_aabb(const math::aabb aabb[],
                          const math::transform transform[],
                          math::aabb out_aabb[],
                          const uint32_t count)
{
  for(uint32_t i = 0; i < count; ++i)
  {
    math::aabb box_copy(aabb[i]);
    math::aabb_scale(box_copy, transform[i].scale);
    math::aabb_set_origin(box_copy, transform[i].position);
    
    out_aabb[i] = box_copy;
  }
}


void
calculate_overlapping_aabbs()
{
}


} // ns