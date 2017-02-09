#include "../graph.hpp"
#include "graph_data.hpp"
#include <math/transform/transform.hpp>
#include <utilities/logging.hpp>
#include <utilities/assert.hpp>


namespace Data {
namespace Graph {


bool
components_set(Graph_data *graph,
               const uint32_t node,
               const uint32_t bitfield)
{
  // -- Param Checks -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uint32_t *bit_fields = (uint32_t*)util::buffer::bytes(&graph->node_components);
    UTIL_ASSERT(bit_fields);
    
    bit_fields[index] = bitfield;
    
    return true;
  }
  
  return false;
}


bool
components_get(const Graph_data *graph,
               const uint32_t node,
               uint32_t *bitfield)
{
  // -- Param Checks -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(bitfield);

  // -- Get Data -- //
  size_t index = 0;

  if(graph->find_index(node, &index))
  {
    const uint32_t *bit_fields = (const uint32_t*)util::buffer::bytes(&graph->node_components);
    UTIL_ASSERT(bit_fields);
    
    *bitfield = bit_fields[index];
    
    return true;
  }

  return false;
}


} // ns
} // ns