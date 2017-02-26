#include "../graph.hpp"
#include "graph_data.hpp"
#include <math/transform/transform.hpp>
#include <utilities/logging.hpp>
#include <utilities/assert.hpp>


namespace Data {
namespace Graph {


bool
transform_set_callback(Graph_data *graph,
                       const uint32_t node,
                       const transform_callback_fn callback)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(callback);
  
  return false;
}


bool
transform_set(Graph_data *graph,
              const uint32_t node,
              const math::transform transform)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);

  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    math::transform *curr_trans(
      (math::transform*)lib::buffer::at(&graph->node_transform, index)
    );
    
    LIB_ASSERT(curr_trans);
    
    *curr_trans = transform;
    
    // Callback //
    
    LOG_TODO_ONCE("Callbacks");
    
    return true;
  }
  
  return false;
}
  
  
bool
transform_get(const Graph_data *graph,
              const uint32_t node,
              math::transform *out_transform)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);

  // -- Get Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    math::transform *curr_trans(
      (math::transform*)lib::buffer::at(&graph->node_transform, index)
    );
    
    LIB_ASSERT(curr_trans);
    
    *out_transform = *curr_trans;
    
    return true;
  }
  
  return false;
}
  

} // ns
} // ns
