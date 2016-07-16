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
                util::generic_id * out_ids[],
                size_t *out_size)
{
  // Do the sweep and prune stages.
  // This will give us the indexes of the colliding objects.
  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, number_of_bounds);
  Physics::Broadphase::sweep_calculate(&sweep, colliders, number_of_bounds);
  
  Physics::Broadphase::Prune prune;
  Physics::Broadphase::prune_init(&prune, &sweep);
  Physics::Broadphase::prune_calculate(&prune, &sweep);

  // Prune out
  static int32_t size_hint = 2048; // Huristic below will increase this size.

  int32_t count = 0;
  util::generic_id *result_id = SCRATCH_ALLOC(util::generic_id, size_hint);
  Physics::Collision::Axis_collidable *result_collidable = SCRATCH_ALLOC(Physics::Collision::Axis_collidable, size_hint);
  
  uint32_t prune_stack = 0;
  
  for(uint32_t i = 0; i < number_of_bounds; ++i)
  {
    if(prune_stack < prune.size && i == prune.ids[prune_stack])
    {
      ++prune_stack;
      
      continue;
    }
    
    result_id[count] = ids[i];
    result_collidable[count] = Physics::Collision::Axis_collidable{{collision_ids[i]}, colliders[i]};
    
    ++count;
  }
  
  // Huristic to make sure we allocate
  // enough space in the next frame
  LOG_TODO_ONCE("Don't like this huristic - could we have a open scratch alloc, where we can keep writting till we signal close?")
  if(math::abs(size_hint - count) < size_hint / 4)
  {
    size_hint *= 2;
  }

  // Results.
  *out_axis_collidables = result_collidable;
  *out_ids = result_id;
  *out_size = count;
  
  // Free these things, incase they are not
  // using scratch allocations.
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