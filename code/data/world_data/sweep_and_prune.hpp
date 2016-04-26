#ifndef OCTREE_POOL_INCLUDED_41874E84_1CD6_4FF0_8271_095C73E61482
#define OCTREE_POOL_INCLUDED_41874E84_1CD6_4FF0_8271_095C73E61482


#include <stdint.h>
#include <core/entity_id.hpp>
#include <math/geometry/aabb.hpp>


#define DATA_OCTREE_LEAF_SIZE 10
#define DATA_OCTREE_MAX_LEAFS 256


#include <vector>


namespace World_data {


struct Bucket
{
  math::aabb bounds;

  Core::Entity_id id[64];
  math::aabb entity_bounds[64];
  
  uint32_t size = 0;
  const uint32_t capacity = 64;
};


struct Sweep_and_prune
{
  Bucket bucket[8 * 8 * 8];
};


void
sweep_and_prune_create(Sweep_and_prune *mark_and_sweep,
                       const Core::Entity_id id[],
                       const math::aabb bounds[],
                       const uint32_t number_of_entities,
                       const math::aabb bounds_size);


} // ns


#endif // inc guard