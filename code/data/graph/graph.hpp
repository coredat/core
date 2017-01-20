#ifndef GRAPH_INCLUDED_57059734_41C1_41D5_BA40_D2092BCFAA41
#define GRAPH_INCLUDED_57059734_41C1_41D5_BA40_D2092BCFAA41


#include "fwd.hpp"
#include <stdint.h>
#include <math/transform/transform_fwd.hpp>
#include <math/geometry/geometry_fwd.hpp>
#include <utilities/alloc.hpp>
#include <stdint.h>
#include <stdlib.h>


namespace Data {
namespace Graph {


// --------------------------------------------------------[ Graph Lifetime ]--


Graph_data*
initialize(const size_t inital_graph_size = 1024,
           const util::malloc_fn malloc_fn = malloc,
           const util::realloc_fn realloc_fn = realloc,
           const util::free_fn free_fn = free);


bool
destroy(Graph_data *graph);


// -------------------------------------------------------[ Node Management ]--


uint32_t
node_add(Graph_data *graph);


bool
node_remove(Graph_data *graph, const uint32_t node);


size_t
node_count(Graph_data *graph);


// -----------------------------------------------[ Node Component Bitfield ]--


bool
components_set(Graph_data *graph,
               const uint32_t node,
               const uint32_t bitfield);


bool
components_get(Graph_data *graph,
               const uint32_t node,
               uint32_t *bitfield);


// -------------------------------------------------------[ Node Properties ]--


bool
tags_set(Graph_data *graph, const uint32_t node, const uint32_t tags);


bool
tags_get(Graph_data *graph, const uint32_t node, uint32_t *tags);


bool
user_data_set(Graph_data *graph,
              const uint32_t node,
              const uintptr_t user_data);


bool
user_data_get(Graph_data *graph,
              const uint32_t node,
              uintptr_t *user_data);


// -------------------------------------------------------[ Node Transforms ]--


bool
transform_set_callback(Graph_data *graph,
                       const uint32_t node,
                       const transform_callback_fn callback);


bool
transform_set(Graph_data *graph,
              const uint32_t node,
              const math::transform transform);
  
  
bool
transform_get(Graph_data *graph,
              const uint32_t node,
              math::transform *out_transform);
  
  
// -------------------------------------------------------------[ Node AABB ]--
  
  
bool
aabb_set(Graph_data *graph,
         const uint32_t node,
         const math::aabb aabb);


bool
aabb_get(Graph_data *graph,
         const uint32_t node,
         const math::aabb *aabb);
  

  
} // ns
} // ns



#endif // inc guard