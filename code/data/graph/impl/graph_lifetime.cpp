#include "../graph.hpp"
#include "graph_data.hpp"
#include <math/transform/transform_types.hpp>
#include <math/geometry/geometry_types.hpp>
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>
#include <new>


namespace Data {
namespace Graph {


Graph_data*
initialize(const size_t inital_graph_size,
           const util::malloc_fn malloc_fn,
           const util::realloc_fn realloc_fn,
           const util::free_fn free_fn)
{
  // -- Param Check -- //
  UTIL_ASSERT(inital_graph_size);
  UTIL_ASSERT(malloc_fn);
  UTIL_ASSERT(realloc_fn);
  UTIL_ASSERT(free_fn);
  
  // -- We do this so we don't have to expose the internals -- //
  void *data = malloc_fn(sizeof(Graph_data));
  Graph_data *graph = new(data) Graph_data();
  
  graph->node_instance_counter = 1; // Root node is 1;
  
  // -- Initalize buffers and get memory -- //
  {
    bool setup = true;
    
    setup &= util::buffer::init(&graph->node_ids,
                                sizeof(uint32_t),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);

    setup &= util::buffer::init(&graph->node_components,
                                sizeof(uint32_t),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);
    
    setup &= util::buffer::init(&graph->node_transform,
                                sizeof(math::transform),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);

    setup &= util::buffer::init(&graph->node_aabb,
                                sizeof(math::aabb),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);

    setup &= util::buffer::init(&graph->node_tags,
                                sizeof(uint32_t),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);

    setup &= util::buffer::init(&graph->node_user_data,
                                sizeof(uintptr_t),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);

    setup &= util::buffer::init(&graph->node_collision_callbacks,
                                sizeof(Graph_callback),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);
    
    setup &= util::buffer::init(&graph->node_message_callbacks,
                                sizeof(Graph_callback),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);

    setup &= util::buffer::init(&graph->node_callbacks,
                                sizeof(uintptr_t),
                                inital_graph_size,
                                malloc_fn, realloc_fn, free_fn);
    
    if(!setup)
    {
      UTIL_ASSERT(false);
      LOG_ERROR("Failed to setup Scene Graph");
    
      destroy(graph);
      
      return nullptr;
    }
  }
  
  LOG_INFO("Scene Graph setup successfully");
  
  return graph;
}


bool
destroy(Graph_data *graph)
{
  if(graph)
  {
    util::buffer::destroy(&graph->node_ids);
    util::buffer::destroy(&graph->node_components);
    util::buffer::destroy(&graph->node_transform);
    util::buffer::destroy(&graph->node_aabb);
    util::buffer::destroy(&graph->node_user_data);
    util::buffer::destroy(&graph->node_collision_callbacks);
    util::buffer::destroy(&graph->node_message_callbacks);
    util::buffer::destroy(&graph->node_callbacks);

    LOG_INFO("Scene Graph destroyed");
    
    return true;
  }
  
  return false;
}


} // ns
} // ns