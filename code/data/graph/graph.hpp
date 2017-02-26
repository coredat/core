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
           const lib::malloc_fn malloc_fn = malloc,
           const lib::realloc_fn realloc_fn = realloc,
           const lib::free_fn free_fn = free);


bool
destroy(Graph_data *graph);


// -------------------------------------------------------------[ Node Data ]--


const uint32_t*
get_node_ids(const Graph_data *graph);


const uint32_t*
get_components(const Graph_data *graph);


const uint32_t*
get_node_tags(const Graph_data *graph);


const math::transform*
get_transforms(const Graph_data *graph);


const math::aabb*
get_aabbs(const Graph_data *graph);


// -------------------------------------------------------[ Node Management ]--


uint32_t
node_add(Graph_data *graph);


bool
node_remove(Graph_data *graph, const uint32_t node);


bool
node_exists(const Graph_data *graph, const uint32_t node);


bool
node_add_destroy_callback(Graph_data *graph, const uintptr_t callback);


bool
node_remove_destroy_callback(Graph_data *graph, const uintptr_t callback);


bool
node_add_collision_callback(Graph_data *graph,
                            const uint32_t node,
                            const uintptr_t user_data,
                            const uintptr_t callback);
  

bool
node_get_collision_callback(const Graph_data *graph,
                            const uint32_t node,
                            uintptr_t *user_data,
                            uintptr_t *callback);


bool
node_remove_collision_callback(Graph_data *graph, const uint32_t node);


size_t
node_count(const Graph_data *graph);


// -----------------------------------------------[ Node Component Bitfield ]--


bool
components_set(Graph_data *graph,
               const uint32_t node,
               const uint32_t bitfield);


bool
components_get(const Graph_data *graph,
               const uint32_t node,
               uint32_t *bitfield);


// -------------------------------------------------------[ Node Properties ]--


bool
tags_set(Graph_data *graph, const uint32_t node, const uint32_t tags);


bool
tags_get(Graph_data *graph, const uint32_t node, uint32_t *tags);


bool
message_callback_set(Graph_data *graph,
                     const uint32_t node,
                     const uintptr_t user_data,
                     const uintptr_t callback_fn);


bool
message_callback_get(const Graph_data *graph,
                     const uint32_t node,
                     uintptr_t *user_data,
                     uintptr_t *callback_fn);


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
transform_get(const Graph_data *graph,
              const uint32_t node,
              math::transform *out_transform);
  
  
// -------------------------------------------------------------[ Node AABB ]--
  
  
bool
aabb_set(Graph_data *graph,
         const uint32_t node,
         const math::aabb aabb);


bool
aabb_get(const Graph_data *graph,
         const uint32_t node,
         math::aabb *aabb);
  

  
} // ns
} // ns



#endif // inc guard
