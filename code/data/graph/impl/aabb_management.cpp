#include "../graph.hpp"
#include "graph_data.hpp"
#include <math/geometry/geometry_types.hpp>
#include <utilities/logging.hpp>
#include <utilities/assert.hpp>


namespace Data {
namespace Graph {


bool
aabb_set(Graph_data *graph,
         const uint32_t node,
         const math::aabb aabb)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    math::aabb *aabbs(
      (math::aabb *)util::buffer::bytes(&graph->node_aabb)
    );
    
    UTIL_ASSERT(aabbs);
    
    aabbs[index] = aabb;
    
    return true;
  }
  
  return false;
}


bool
aabb_get(const Graph_data *graph,
         const uint32_t node,
         math::aabb *aabb)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(aabb);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    const math::aabb *aabbs(
      (const math::aabb *)util::buffer::bytes(&graph->node_aabb)
    );
    
    UTIL_ASSERT(aabbs);
    
    *aabb = aabbs[index];
    
    return true;
  }
  
  return false;
}


} // ns
} // ns