#include "../graph.hpp"
#include "graph_data.hpp"
#include <utilities/logging.hpp>
#include <utilities/assert.hpp>


namespace Data {
namespace Graph {


bool
tags_set(Graph_data *graph, const uint32_t node, const uint32_t tags)
{
  // -- Param Checks -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(tags);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uint32_t *curr_tags(
      (uint32_t *)lib::buffer::bytes(&graph->node_tags)
    );
    
    LIB_ASSERT(curr_tags);
    
    curr_tags[index] = tags;
    
    return true;
  }
  
  return false;
}


bool
tags_get(Graph_data *graph, const uint32_t node, uint32_t *tags)
{
 // -- Param Checks -- //
 LIB_ASSERT(graph);
 LIB_ASSERT(node);
 LIB_ASSERT(tags);
 
  // -- Get Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uint32_t *curr_tags(
      (uint32_t *)lib::buffer::bytes(&graph->node_tags)
    );
    
    LIB_ASSERT(curr_tags);
    
    *tags = curr_tags[index];
    
    return true;
  }
  
  return false;
}


bool
message_callback_set(Graph_data *graph,
                     const uint32_t node,
                     const uintptr_t user_data,
                     const uintptr_t callback_fn)
{
  // -- Param Checks -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    Graph_callback *graph_cb(
      (Graph_callback*)lib::buffer::bytes(&graph->node_message_callbacks)
    );
    
    LIB_ASSERT(graph_cb);
    
    graph_cb[index].user_data = user_data;
    graph_cb[index].function_ptr = callback_fn;
    
    return true;
  }

  return false;
}


bool
message_callback_get(const Graph_data *graph,
                     const uint32_t node,
                     uintptr_t *user_data,
                     uintptr_t *callback_fn)
{
  // -- Param Checks -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(user_data);
  LIB_ASSERT(callback_fn);
  
  // -- Get Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    Graph_callback *graph_cb(
      (Graph_callback*)lib::buffer::bytes(&graph->node_message_callbacks)
    );
    
    LIB_ASSERT(graph_cb);
    
    *user_data = graph_cb[index].user_data;
    *callback_fn = graph_cb[index].function_ptr;
    
    return true;
  }
  
  return false;
}


bool
user_data_set(Graph_data *graph,
              const uint32_t node,
              const uintptr_t user_data)
{
  // -- Param Checks -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(user_data);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uintptr_t *curr_user_data(
      (uintptr_t *)lib::buffer::bytes(&graph->node_user_data)
    );
    
    LIB_ASSERT(curr_user_data);
    
    curr_user_data[index] = user_data;
    
    return true;
  }
  
  return false;
}
  
  
bool
user_data_get(Graph_data *graph,
              const uint32_t node,
              uintptr_t *user_data)
{
  // -- Param Checks -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(user_data);
  
  // -- Get Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uintptr_t *curr_user_data(
      (uintptr_t *)lib::buffer::bytes(&graph->node_user_data)
    );
    
    LIB_ASSERT(curr_user_data);
    
    *user_data = curr_user_data[index];
    
    return true;
  }
  
  return false;
}


} // ns
} // ns
