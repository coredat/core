#include "../graph.hpp"
#include "graph_data.hpp"
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>


namespace Data {
namespace Graph {


uint32_t
node_add(Graph_data *graph)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  
  // -- Add New Node -- //

  const uint32_t new_instance = graph->node_instance_counter++;
  
  util::buffer::push(&graph->node_ids);
  uint32_t *id = (uint32_t*)util::buffer::last(&graph->node_ids);
  *id = new_instance;
  
  util::buffer::push(&graph->node_callbacks);
  util::buffer::push(&graph->node_aabb);
  util::buffer::push(&graph->node_components);
  util::buffer::push(&graph->node_tags);
  util::buffer::push(&graph->node_transform);
  util::buffer::push(&graph->node_user_data);
  util::buffer::push(&graph->node_user_callbacks);
  
  // Cache as following calls will likely setup the properties //
  graph->last_instance = new_instance;
  graph->index_cache = util::buffer::size(&graph->node_ids);
  
  return new_instance;
}


bool
node_remove(Graph_data *graph,
            const uint32_t node)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  
  // -- Remove Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    util::buffer::erase(&graph->node_ids, index);
    util::buffer::erase(&graph->node_aabb, index);
    util::buffer::erase(&graph->node_components, index);
    util::buffer::erase(&graph->node_tags, index);
    util::buffer::erase(&graph->node_transform, index);
    util::buffer::erase(&graph->node_user_data, index);
    util::buffer::erase(&graph->node_user_callbacks, index);
    
    // Invalidate the cache //
    graph->last_instance = -1;
    graph->index_cache = -1;
    
    return true;
  }
  
  LOG_ERROR("Failed to find entity");
  
  return false;
}


size_t
node_count(Graph_data *graph)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);

  return util::buffer::size(&graph->node_ids);
}


} // ns
} // ns