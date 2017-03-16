#include "../graph.hpp"
#include "graph_data.hpp"
#include <utilities/utilities.hpp>
#include <math/math.hpp>


// -------------------------------------------------------[ Node Management ]--


namespace Data {
namespace Graph {


uint32_t
node_add(Graph_data *graph, const uint32_t parent_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  
  // -- Find parent -- //
  size_t insert_point = 0;
  {
    const uint32_t *node_data = graph->node_ids.data();
    const size_t    node_count = graph->node_ids.size();
  
    if(lib::key::linear_search(
        parent_id,
        node_data,
        node_count,
        &insert_point))
    {
      insert_point += 1;
    }
    else
    {
      insert_point = graph->node_ids.size();
    }
  }
  
  // -- Insert New Entity -- //
  const uint32_t new_instance = ++graph->node_instance_counter;
  
  const size_t node_count = graph->node_ids.size();
  
  if(insert_point < node_count)
  {
    graph->node_ids.insert(
      insert_point,
      new_instance
    );
    
    graph->parent_ids.insert(
      insert_point,
      parent_id
    );
    
    graph->node_callbacks.insert(
      insert_point,
      uintptr_t{0},
      uintptr_t{0}
    );
    
    graph->node_aabb.insert(
      insert_point,
      math::vec3_zero(),
      math::vec3_zero()
    );
    
    graph->node_components.insert(
      insert_point,
      uint32_t{0}
    );
    
    graph->node_tags.insert(
      insert_point,
      uint32_t{0}
    );
    
    graph->node_world_transform.insert(
      insert_point,
      math::vec3_zero(),
      math::vec3_one(),
      math::quat_init()
    );
    
    graph->node_world_transform.insert(
      insert_point,
      math::vec3_zero(),
      math::vec3_one(),
      math::quat_init()
    );
    
    graph->node_user_data.insert(
      insert_point,
      uintptr_t{0}
    );
    
    graph->node_collision_callbacks.insert(
      insert_point,
      uintptr_t{0},
      uintptr_t{0}
    );

    graph->node_message_callbacks.insert(
      insert_point,
      uintptr_t{0},
      uintptr_t{0}
    );
  }
  else
  {
    graph->node_ids.emplace_back(
      new_instance
    );
    
    graph->parent_ids.emplace_back(
      parent_id
    );
    
    graph->node_callbacks.emplace_back(
      uintptr_t{0},
      uintptr_t{0}
    );
    
    graph->node_aabb.emplace_back(
      math::vec3_zero(),
      math::vec3_zero()
    );
    
    graph->node_components.emplace_back(
      uint32_t{0}
    );
    
    graph->node_tags.emplace_back(
      uint32_t{0}
    );
    
    graph->node_world_transform.emplace_back(
      math::vec3_zero(),
      math::vec3_one(),
      math::quat_init()
    );
    
    graph->node_world_transform.emplace_back(
      math::vec3_zero(),
      math::vec3_one(),
      math::quat_init()
    );
    
    graph->node_user_data.emplace_back(
      uintptr_t{0}
    );
    
    graph->node_collision_callbacks.emplace_back(
      uintptr_t{0},
      uintptr_t{0}
    );

    graph->node_message_callbacks.emplace_back(
      uintptr_t{0},
      uintptr_t{0}
    );
  }
  
  // Cache as following calls will likely setup the properties //
  graph->last_instance = new_instance;
  graph->index_cache = graph->node_ids.size() - 1;
  
  return new_instance;
}


bool
node_remove(Graph_data *graph,
            const uint32_t node)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  
  // -- Remove Data -- //
  size_t index = 0;
  
  if(graph->find_index(node, &index))
  {
    graph->node_ids.erase(index);
    graph->parent_ids.erase(index);
    graph->node_aabb.erase(index);
    graph->node_components.erase(index);
    graph->node_tags.erase(index);
    graph->node_world_transform.erase(index);
    graph->node_local_transform.erase(index);
    graph->node_user_data.erase(index);
    graph->node_collision_callbacks.erase(index);
    graph->node_message_callbacks.erase(index);
    
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
  LIB_ASSERT(graph);
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
  LIB_ASSERT(graph);

  return graph->node_ids.size();
}


bool
node_add_collision_callback(Graph_data *graph,
                            const uint32_t node,
                            const uintptr_t user_data,
                            const uintptr_t callback)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(callback);
  
  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    Graph_callback *callbacks(
      graph->node_collision_callbacks.data()
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
  LIB_ASSERT(graph);
  LIB_ASSERT(node);
  LIB_ASSERT(user_data);
  LIB_ASSERT(callback);

  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    const Graph_callback *callbacks(
      graph->node_collision_callbacks.data()
    );
    
    *callback  = callbacks[index].function_ptr;
    *user_data = callbacks[index].user_data;
    
    return true;
  }
  
  return false;
}


bool
node_remove_collision_callback(Graph_data *graph, const uint32_t node)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node);

  size_t index = 0;
  if(graph->find_index(node, &index))
  {
    Graph_callback *callbacks(
      graph->node_collision_callbacks.data()
    );
    
    callbacks[index].function_ptr = 0;
    callbacks[index].user_data    = 0;
    
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
  return graph->node_ids.data();
}


const uint32_t*
get_components(const Graph_data *graph)
{
  return graph->node_components.data();
}


const uint32_t*
get_node_tags(const Graph_data *graph)
{
  return graph->node_tags.data();
}


const uintptr_t*
get_user_data(const Graph_data *graph)
{
  return graph->node_user_data.data();
}


const math::transform*
get_world_transforms(const Graph_data *graph)
{
  return graph->node_world_transform.data();
}


const math::transform*
get_local_transforms(const Graph_data *graph)
{
  return graph->node_local_transform.data();
}


const math::aabb*
get_aabbs(const Graph_data *graph)
{
  return graph->node_aabb.data();
}


} // ns
} // ns
