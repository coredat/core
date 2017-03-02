#include "../graph.hpp"
#include "graph_data.hpp"
#include <math/transform/transform.hpp>
#include <utilities/logging.hpp>
#include <utilities/assert.hpp>


namespace Data {
namespace Graph {


uint32_t
transform_set_callback(Graph_data *graph,
                       const uintptr_t user_data,
                       const transform_callback_fn callback)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(callback);
  
  graph->transform_callbacks.emplace_back(user_data, (uintptr_t)callback);
  
  return graph->transform_callbacks.size();
}


bool
transform_set(
  Graph_data *graph,
  const uint32_t node,
  const math::transform *transform,
  const uint32_t from_callback)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);

  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    math::transform *curr_trans(
      graph->node_transform.data()
    );
    
    LIB_ASSERT(curr_trans);
    
    curr_trans[index] = *transform;
    
    // Callback //
    for(uint32_t i = 0; i < (uint32_t)graph->transform_callbacks.size(); ++i)
    {
      if(i != from_callback - 1)
      {
        Graph_callback* cb = graph->transform_callbacks.data();
        ((transform_callback_fn)(cb[i].function_ptr))(
          node,
          transform,
          cb[i].user_data
        );
      }
    }
    
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
    const math::transform *curr_trans(
      graph->node_transform.data()
    );
    
    LIB_ASSERT(curr_trans);
    
    *out_transform = curr_trans[index];
    
    return true;
  }
  
  return false;
}
  

} // ns
} // ns
