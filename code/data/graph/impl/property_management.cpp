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
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(tags);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uint32_t *curr_tags(
      (uint32_t *)util::buffer::at(&graph->node_tags, index)
    );
    
    UTIL_ASSERT(curr_tags);
    
    *curr_tags = tags;
    
    return true;
  }
  
  return false;
}


bool
tags_get(Graph_data *graph, const uint32_t node, uint32_t *tags)
{
 // -- Param Checks -- //
 UTIL_ASSERT(graph);
 UTIL_ASSERT(node);
 UTIL_ASSERT(tags);
 
  // -- Get Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uint32_t *curr_tags(
      (uint32_t *)util::buffer::at(&graph->node_tags, index)
    );
    
    UTIL_ASSERT(curr_tags);
    
    *curr_tags = *tags;
    
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
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(user_data);
  
  // -- Get and Set Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uintptr_t *curr_user_data(
      (uintptr_t *)util::buffer::at(&graph->node_user_data, index)
    );
    
    UTIL_ASSERT(curr_user_data);
    
    *curr_user_data = user_data;
    
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
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(user_data);
  
  // -- Get Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    uintptr_t *curr_user_data(
      (uintptr_t *)util::buffer::at(&graph->node_user_data, index)
    );
    
    UTIL_ASSERT(curr_user_data);
    
    *curr_user_data = *user_data;
    
    return true;
  }
  
  return false;
}



} // ns
} // ns