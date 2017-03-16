#include "../graph.hpp"
#include "graph_data.hpp"
#include <utilities/utilities.hpp>
#include <math/math.hpp>


// -------------------------------------------------------- [ Graph Config ] --


#define GRAPH_INLINE inline


// ------------------------------------------------------- [ Graph Helpers ] --


GRAPH_INLINE uint32_t
get_parent_id(const uint64_t data)
{
  return lib::bits::upper32(data);
}

GRAPH_INLINE uint32_t
get_depth(const uint64_t data)
{
  return lib::bits::lower32(data);
}

GRAPH_INLINE uint64_t
set_data(const uint32_t parent, const uint32_t depth)
{
  return lib::bits::pack3232(depth, parent);
}

GRAPH_INLINE uint64_t
set_parent_id(const uint64_t data, const uint32_t parent_id)
{
  return lib::bits::pack3232(
    get_depth(data),
    parent_id   
  );
}

GRAPH_INLINE uint64_t
set_depth(const uint64_t data, const uint32_t depth)
{
  return lib::bits::pack3232(
    depth,
    get_parent_id(data)
  );
}



namespace {

/*
  The number of node (and data) that will be pushed onto the stack to avoid
  a heap allocation.
*/
constexpr size_t stack_hint = 32;

/*
  The default transform if none is specified.
*/
const math::transform default_transform(
  math::transform_init()
);


} // anon ns


// -------------------------------------------------------[ Node Management ]--


namespace Data {
namespace Graph {


uint32_t
node_add(Graph_data *graph)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  
  // -- Generate a new instance and find insert point -- //
  const uint32_t new_instance = ++graph->node_instance_counter;
  size_t         insert_point = graph->node_ids.size();
  uint32_t       depth        = 0;
  
  // -- Local Transform -- //
  const math::transform *trans = local_trans ? local_trans : &default_transform;
  
  // -- Insert new data at the end -- //
  const uint64_t data = set_data(0, depth); 
  graph->node_ids.insert(insert_point, new_instance);
  graph->node_data.insert(insert_point, data);
  graph->local_transforms.insert(insert_point, *trans);
  graph->world_transforms.insert(insert_point, *trans);

  return new_instance;
}


bool
node_set_parent(
  Graph_data *graph,
  const uint32_t this_id,
  const uint32_t parent)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(this_id);
  
  // -- Find this entities details -- //
  size_t this_index = 0;
  uint32_t this_depth = 0;
  {
    if(!lib::key::linear_search(
      this_id,
      graph->node_ids.data(),
      graph->node_ids.size(),
      &this_index))
    {
      return false;
    }

    this_depth = get_depth(graph->node_data[this_index]);
  }

  // -- Find out how many nodes we need to move -- //
  uint32_t nodes_to_move = 1;
  {
    const size_t node_count = graph->node_ids.size();
    const size_t start_index = this_index + 1;

    for(size_t i = start_index; i < node_count; ++i, ++nodes_to_move)
    {
      const uint32_t curr_depth = get_depth(graph->node_data[i]);
    
      if(curr_depth <= this_depth)
      {
        break;
      }
    }
  }

  // -- Remove nodes, and insert else where in the tree -- //
  {
    // Save the old data
    lib::array<uint32_t, stack_hint> move_nodes(
      graph->node_ids.begin() + this_index,
      graph->node_ids.begin() + (this_index + nodes_to_move)
    );
    graph->node_ids.erase(this_index, nodes_to_move);
    
    lib::array<uint64_t, stack_hint> move_node_data(
      graph->node_data.begin() + this_index,
      graph->node_data.begin() + (this_index + nodes_to_move)
    );
    graph->node_data.erase(this_index, nodes_to_move);
    
    lib::array<math::transform, stack_hint> move_local_transforms(
      graph->local_transforms.begin() + this_index,
      graph->local_transforms.begin() + (this_index + nodes_to_move)
    );
    graph->local_transforms.erase(this_index, nodes_to_move);
    
    lib::array<math::transform, stack_hint> move_world_transforms(
      graph->world_transforms.begin() + this_index,
      graph->world_transforms.begin() + (this_index + nodes_to_move)
    );
    graph->world_transforms.erase(this_index, nodes_to_move);
    
    // Find new insert point
    size_t parent_index   = 0;
    size_t insert_index   = graph->node_data.size();
    uint32_t parent_depth = 0;
    {
      if(parent_id > 0)
      {
        if(!lib::key::linear_search(
          parent_id,
          graph->nodes.data(),
          graph->nodes.size(),
          &parent_index))
        {
          return false;
        }

        insert_index = parent_index + 1;
        parent_depth = get_depth(graph->node_data[parent_index]); 
      }
    }
    
    // Change the first node to point at the new parent.
    move_node_data[0] = set_data(parent_id, get_depth(move_node_data[0]));
    
    /*
      Insert the data into the new positions.
    */
    graph->nodes.insert(
      insert_index,
      move_nodes.data(),
      nodes_to_move
    );
    
    graph->node_data.insert(
      insert_index,
      move_node_data.data(),
      nodes_to_move
    );
    
    graph->local_transforms.insert(
      insert_index,
      move_local_transforms.data(),
      nodes_to_move
    );
    
    graph->world_transforms.insert(
      insert_index,
      move_world_transforms.data(),
      nodes_to_move
    );
    
    /*
      Calculate the depth differences between old and new parents.
      If parent_id = 0 aka root node, we need to add an extra -1 to get the
      correct depth.
    */
    const int32_t old_parent_depth = this_depth - 1;
    const int32_t parent_diff      = parent_depth - old_parent_depth;
    const int32_t depth_diff       = parent_diff - (parent_id ? 0 : 1);
    
    // Update the depth data
    for(uint32_t i = 0; i < nodes_to_move; ++i)
    {
      const uint64_t old_data  = move_node_data[i];
      const uint32_t old_depth = get_depth(old_data);
      const uint32_t depth     = old_depth + depth_diff;
      const uint64_t new_data  = set_data(get_parent_id(old_data), depth);
      const size_t   index     = insert_index + i;
      
      graph->node_data[index] = new_data;
    }
    
    // -- Update transforms -- //
    /*
      Transforms are nested so we keep a stack of transforms that represent
      the parent transform, we pop off when we walk up the tree.
    */
    {
      lib::array<math::transform, stack_hint> transform_stack;
      transform_stack.emplace_back(graph->world_transforms[parent_index]);
      
      uint32_t curr_depth = parent_depth;
      
      for(uint32_t i = 0; i < nodes_to_move; ++i)
      {
        const size_t index  = insert_index + i;
        const uint64_t data = graph->node_data[index];
        const uint32_t depth = get_depth(data);
        
        // Pop off all unrequired transforms.
        while((curr_depth + 1) < depth)
        {
          transform_stack.pop_back();
          curr_depth -= 1;
        }
        
        curr_depth = depth;
      
        // Calc new world transform.
        const math::transform child_world(
          math::transform_inherited(
            transform_stack.top(),
            graph->local_transforms[index]
          )
        );
        
        graph->world_transforms[index] = child_world;
        
        transform_stack.emplace_back(child_world);
      }
    }
  }

  return true;
}


uint32_t
node_get_parent(Graph_data *graph, const uint32_t node)
{
  size_t parent_id = 0;
  size_t index = 0;
  
  if(lib::key::linear_search(
      this_id,
      graph->node_ids.data(),
      graph->node_ids.size(),
      &index)
    )
  {
    parent_id = get_parent_id(graph->node_data[index]);
  }
  
  return parent_id;
}


uint32_t
node_get_child_count(Graph_data *graph, const uint32_t node)
{
  size_t child_count = 0;
  size_t index = 0;
  
  if(lib::key::linear_search(
      this_id,
      graph->node_ids.data(),
      graph->node_ids.size(),
      &index))
  {
    const uint32_t this_depth = get_depth(graph->node_data[index]);
    
    for(size_t i = index; i < graph->node_data.size(); ++i)
    {
      if(get_depth(graph->node_data[i]) > this_depth)
      {
        ++child_count;
      }
    }
  }
  
  return child_count;
}


uint32_t
node_get_child(Graph_data *graph, const uint32_t node, const size_t i)
{
}


bool
node_remove(Graph_data *graph, const uint32_t node)
{
}


bool
node_exists(const Graph_data *graph, const uint32_t node)
{
}


//uint32_t
//node_add(Graph_data *graph, const uint32_t parent_id)
//{
//  // -- Param Check -- //
//  LIB_ASSERT(graph);
//  
//  // -- Find parent -- //
//  size_t insert_point = 0;
//  {
//    const uint32_t *node_data = graph->node_ids.data();
//    const size_t    node_count = graph->node_ids.size();
//  
//    if(lib::key::linear_search(
//        parent_id,
//        node_data,
//        node_count,
//        &insert_point))
//    {
//      insert_point += 1;
//    }
//    else
//    {
//      insert_point = graph->node_ids.size();
//    }
//  }
//  
//  // -- Insert New Entity -- //
//  const uint32_t new_instance = ++graph->node_instance_counter;
//  
//  const size_t node_count = graph->node_ids.size();
//  
//  if(insert_point < node_count)
//  {
//    graph->node_ids.insert(
//      insert_point,
//      new_instance
//    );
//    
//    graph->parent_ids.insert(
//      insert_point,
//      parent_id
//    );
//    
//    graph->node_callbacks.insert(
//      insert_point,
//      uintptr_t{0},
//      uintptr_t{0}
//    );
//    
//    graph->node_aabb.insert(
//      insert_point,
//      math::vec3_zero(),
//      math::vec3_zero()
//    );
//    
//    graph->node_components.insert(
//      insert_point,
//      uint32_t{0}
//    );
//    
//    graph->node_tags.insert(
//      insert_point,
//      uint32_t{0}
//    );
//    
//    graph->node_world_transform.insert(
//      insert_point,
//      math::vec3_zero(),
//      math::vec3_one(),
//      math::quat_init()
//    );
//    
//    graph->node_world_transform.insert(
//      insert_point,
//      math::vec3_zero(),
//      math::vec3_one(),
//      math::quat_init()
//    );
//    
//    graph->node_user_data.insert(
//      insert_point,
//      uintptr_t{0}
//    );
//    
//    graph->node_collision_callbacks.insert(
//      insert_point,
//      uintptr_t{0},
//      uintptr_t{0}
//    );
//
//    graph->node_message_callbacks.insert(
//      insert_point,
//      uintptr_t{0},
//      uintptr_t{0}
//    );
//  }
//  else
//  {
//    graph->node_ids.emplace_back(
//      new_instance
//    );
//    
//    graph->parent_ids.emplace_back(
//      parent_id
//    );
//    
//    graph->node_callbacks.emplace_back(
//      uintptr_t{0},
//      uintptr_t{0}
//    );
//    
//    graph->node_aabb.emplace_back(
//      math::vec3_zero(),
//      math::vec3_zero()
//    );
//    
//    graph->node_components.emplace_back(
//      uint32_t{0}
//    );
//    
//    graph->node_tags.emplace_back(
//      uint32_t{0}
//    );
//    
//    graph->node_world_transform.emplace_back(
//      math::vec3_zero(),
//      math::vec3_one(),
//      math::quat_init()
//    );
//    
//    graph->node_world_transform.emplace_back(
//      math::vec3_zero(),
//      math::vec3_one(),
//      math::quat_init()
//    );
//    
//    graph->node_user_data.emplace_back(
//      uintptr_t{0}
//    );
//    
//    graph->node_collision_callbacks.emplace_back(
//      uintptr_t{0},
//      uintptr_t{0}
//    );
//
//    graph->node_message_callbacks.emplace_back(
//      uintptr_t{0},
//      uintptr_t{0}
//    );
//  }
//  
//  // Cache as following calls will likely setup the properties //
//  graph->last_instance = new_instance;
//  graph->index_cache = graph->node_ids.size() - 1;
//  
//  return new_instance;
//}
//
//
//bool
//node_remove(Graph_data *graph,
//            const uint32_t node)
//{
//  // -- Param Check -- //
//  LIB_ASSERT(graph);
//  LIB_ASSERT(node);
//  
//  // -- Remove Data -- //
//  size_t index = 0;
//  
//  if(graph->find_index(node, &index))
//  {
//    graph->node_ids.erase(index);
//    graph->parent_ids.erase(index);
//    graph->node_aabb.erase(index);
//    graph->node_components.erase(index);
//    graph->node_tags.erase(index);
//    graph->node_world_transform.erase(index);
//    graph->node_local_transform.erase(index);
//    graph->node_user_data.erase(index);
//    graph->node_collision_callbacks.erase(index);
//    graph->node_message_callbacks.erase(index);
//    
//    // Invalidate the cache //
//    graph->last_instance = -1;
//    graph->index_cache = -1;
//    
//    return true;
//  }
//  
//  LOG_ERROR("Failed to find entity");
//  
//  return false;
//}
//
//
//bool
//node_exists(const Graph_data *graph, const uint32_t node)
//{
//  // -- Param Check -- //
//  LIB_ASSERT(graph);
//  // Don't check node, it should be valid to ask if node zero is valid.
//  
//  size_t index = 0;
//  if(graph->find_index(node, &index))
//  {
//    return true;
//  }
//  
//  return false;
//}
//
//
//size_t
//node_count(const Graph_data *graph)
//{
//  // -- Param Check -- //
//  LIB_ASSERT(graph);
//
//  return graph->node_ids.size();
//}


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
