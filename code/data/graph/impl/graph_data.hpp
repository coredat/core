#ifndef GRAPH_DATA_INCLUDED_D0BAF703_5E6E_4A89_B194_85E7210667C2
#define GRAPH_DATA_INCLUDED_D0BAF703_5E6E_4A89_B194_85E7210667C2


#include <utilities/utilities.hpp>
#include <math/math.hpp>
#include <stddef.h>


namespace Data {
namespace Graph {


struct Graph_callback
{
  uintptr_t user_data;
  uintptr_t function_ptr;
};


struct Graph_data
{
  /*
    Graph guts
  */
  lib::array<uint32_t>        node_ids;
  lib::array<uint64_t>        node_data;
  lib::array<math::transform> local_transforms;
  lib::array<math::transform> world_transforms;
  
  /*
    Internal use.
  */
  lib::array<Graph_callback> node_callbacks;
  lib::array<math::aabb> node_aabb;
  lib::array<uint32_t> node_components;
  
  /*
    External and Internal use.
  */
  lib::array<uint32_t> node_tags;
  lib::array<uintptr_t> node_user_data;
  lib::array<Graph_callback> node_collision_callbacks;
  lib::array<Graph_callback> node_message_callbacks;
  
  /*
    Instance counter.
  */
  size_t node_instance_counter = 0;
  
  /*
    Cache last index lookup.
  */
  mutable size_t last_instance = -1;
  mutable size_t index_cache   = 0;

  bool
  find_index(size_t instance, size_t *index) const
  {
//    if(instance == last_instance)
//    {
//      *index = index_cache;
//      return true;
//    }
    
    const uint32_t *ids = node_ids.data();
    size_t count = node_ids.size();
    
    LOG_TODO_ONCE("Make a bin search");
    
    if(lib::key::linear_search(instance, ids, count, &index_cache))
    {
      *index = index_cache;
      return true;
    }
    
    LOG_FATAL("Can't find this Node");
    return false;
  }
  
  lib::array<Graph_callback, 4> transform_callbacks;
};


} // ns
} // ns


#endif // inc guard
