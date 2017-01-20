#ifndef GRAPH_DATA_INCLUDED_D0BAF703_5E6E_4A89_B194_85E7210667C2
#define GRAPH_DATA_INCLUDED_D0BAF703_5E6E_4A89_B194_85E7210667C2


#include <utilities/buffer.hpp>
#include <utilities/id32.hpp>
#include <utilities/logging.hpp>
#include <stddef.h>


namespace Data {
namespace Graph {


struct Graph_data
{
  util::buffer::data node_ids;
  
  /*
    Internal use.
  */
  
  util::buffer::data node_callbacks;
  util::buffer::data node_aabb;
  util::buffer::data node_components;
  
  /*
    External and Internal use.
  */
  
  util::buffer::data node_tags;
  util::buffer::data node_transform;
  util::buffer::data node_user_data;
  util::buffer::data node_user_callbacks;
  
  /*
    Instance counter.
  */
  
  size_t node_instance_counter = 0;
  
  /*
    Cache last index lookup.
  */
  size_t last_instance = -1;
  size_t index_cache   = 0;

  bool
  find_index(size_t instance, size_t *index)
  {
    if(instance == last_instance)
    {
      *index = index_cache;
      return true;
    }
    
    uint32_t *ids = (uint32_t*)util::buffer::bytes(&node_ids);
    size_t count = util::buffer::size(&node_ids);
    
    LOG_TODO_ONCE("Make a bin search");
    
    if(util::id::linear_search(instance, ids, count, &index_cache))
    {
      *index = index_cache;
      return true;
    }
    
    LOG_FATAL("Can't find this Node");
    return false;
  }
};



struct Callbacks
{
};


} // ns
} // ns


#endif // inc guard