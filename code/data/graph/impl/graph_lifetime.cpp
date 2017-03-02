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
           const lib::malloc_fn malloc_fn,
           const lib::realloc_fn realloc_fn,
           const lib::free_fn free_fn)
{
  // -- Param Check -- //
  LIB_ASSERT(inital_graph_size);
  LIB_ASSERT(malloc_fn);
  LIB_ASSERT(realloc_fn);
  LIB_ASSERT(free_fn);
  
  // -- We do this so we don't have to expose the internals -- //
  void *data = malloc_fn(sizeof(Graph_data));
  LIB_ASSERT(data);
  
  Graph_data *graph = new(data) Graph_data();
  LIB_ASSERT(graph);
  
  graph->node_instance_counter = 1; // Root node is 1;
  
  // -- Initalize buffers and get memory -- //
  {
    bool setup = true;
    
    if(!setup)
    {
      LIB_ASSERT(false);
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
    LOG_INFO("Scene Graph destroyed");
    
    return true;
  }
  
  return false;
}


} // ns
} // ns
