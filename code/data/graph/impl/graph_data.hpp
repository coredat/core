#ifndef GRAPH_DATA_INCLUDED_D0BAF703_5E6E_4A89_B194_85E7210667C2
#define GRAPH_DATA_INCLUDED_D0BAF703_5E6E_4A89_B194_85E7210667C2


#include <utilities/buffer.hpp>
#include <utilities/id.hpp>
#include <utilities/logging.hpp>
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
  lib::buffer::data node_ids;
  
  /*
    Internal use.
  */
  lib::buffer::data node_callbacks;
  lib::buffer::data node_aabb;
  lib::buffer::data node_components;
  
  /*
    External and Internal use.
  */
  lib::buffer::data node_tags;
  lib::buffer::data node_transform;
  lib::buffer::data node_user_data;
  lib::buffer::data node_collision_callbacks;
  lib::buffer::data node_message_callbacks;
  
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
    
    const uint32_t *ids = (const uint32_t*)lib::buffer::bytes(&node_ids);
    size_t count = lib::buffer::size(&node_ids);
    
    LOG_TODO_ONCE("Make a bin search");
    
    if(lib::id::linear_search(instance, ids, count, &index_cache))
    {
      *index = index_cache;
      return true;
    }
    
    LOG_FATAL("Can't find this Node");
    return false;
  }
};


} // ns
} // ns


#endif // inc guard
