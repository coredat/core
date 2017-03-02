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
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    math::aabb *aabbs(
      graph->node_aabb.data()
    );
    
    LIB_ASSERT(aabbs);
    
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
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(aabb);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    const math::aabb *aabbs(
      graph->node_aabb.data()
    );
    
    LIB_ASSERT(aabbs);
    
    *aabb = aabbs[index];
    
    return true;
  }
  
  return false;
}


} // ns
} // ns
