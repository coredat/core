#include "../graph.hpp"
#include "graph_data.hpp"
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>


// -------------------------------------------------------[ Node Management ]--


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
  util::buffer::push(&graph->node_user_data);
  util::buffer::push(&graph->node_collision_callbacks);
  
  // Cache as following calls will likely setup the properties //
  graph->last_instance = new_instance;
  graph->index_cache = util::buffer::size(&graph->node_ids) - 1;
  
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
    util::buffer::erase(&graph->node_collision_callbacks, index);
    
    // Invalidate the cache //
    graph->last_instance = -1;
    graph->index_cache = -1;
    
    return true;
  }
  
  LOG_ERROR("Failed to find entity");
  
  return false;
}


bool
node_exists(const Graph_data *graph, const uint32_t node)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  // Don't check node, it should be valid to ask if node zero is valid.
  
  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    return true;
  }
  
  return false;
}


size_t
node_count(const Graph_data *graph)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);

  return util::buffer::size(&graph->node_ids);
}


bool
node_add_collision_callback(Graph_data *graph,
                            const uint32_t node,
                            const uintptr_t user_data,
                            const uintptr_t callback)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(callback);
  
  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    Graph_callback *callbacks(
      (Graph_callback*)util::buffer::bytes(&graph->node_collision_callbacks)
    );
    
    callbacks[index].function_ptr = callback;
    callbacks[index].user_data = user_data;
    
    return true;
  }
  
  return false;
}


bool
node_get_collision_callback(const Graph_data *graph,
                            const uint32_t node,
                            uintptr_t *user_data,
                            uintptr_t *callback)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);
  UTIL_ASSERT(user_data);
  UTIL_ASSERT(callback);

  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    const Graph_callback *callbacks(
      (const Graph_callback*)util::buffer::bytes(&graph->node_collision_callbacks)
    );
    
    *callback = callbacks[index].function_ptr;
    *user_data = callbacks[index].user_data;
    
    return true;
  }
  
  return false;
}


bool
node_remove_collision_callback(Graph_data *graph, const uint32_t node)
{
  // -- Param Check -- //
  UTIL_ASSERT(graph);
  UTIL_ASSERT(node);

  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    Graph_callback *callbacks(
      (Graph_callback*)util::buffer::bytes(&graph->node_collision_callbacks)
    );
    
    callbacks[index].function_ptr = 0;
    callbacks[index].user_data = 0;
    
    return true;
  }
  
  return false;
}


} // ns
} // ns


// -------------------------------------------------------------[ Node Data ]--


namespace Data {
namespace Graph {


const uint32_t*
get_node_ids(const Graph_data *graph)
{
  return (const uint32_t*)util::buffer::bytes(&graph->node_ids);
}


const uint32_t*
get_components(const Graph_data *graph)
{
  return (const uint32_t*)util::buffer::bytes(&graph->node_components);
}


const uint32_t*
get_node_tags(const Graph_data *graph)
{
  return (const uint32_t*)util::buffer::bytes(&graph->node_tags);
}


const math::transform*
get_transforms(const Graph_data *graph)
{
  return (const math::transform*)util::buffer::bytes(&graph->node_transform);
}


const math::aabb*
get_aabbs(const Graph_data *graph)
{
  return (const math::aabb*)util::buffer::bytes(&graph->node_aabb);
}


} // ns
} // ns