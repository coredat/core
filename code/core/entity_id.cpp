#include "entity_id.hpp"
#include <assert.h>
#include <cstring>
#include <math/math.hpp>
#include <utilities/optimizations.hpp>


namespace Core {
namespace Entity_id_util {


void
init_to_invalid_ids(Entity_id ids[], const uint32_t size_of_entities)
{
  memset(ids, 0, size_of_entities * sizeof(Entity_id));
}


bool
find_index_binary(uint32_t *out_index,
                  const Entity_id id_to_find,
                  const Entity_id ids[],
                  const uint32_t number_of_entities)
{
  uint32_t down = 0;
  uint32_t up = number_of_entities - 1;
  uint32_t position = 0;
  
  uint32_t iterations = 0;

  while(up >= down)
  {
    iterations++;
    
    position = (down + up) >> 1;
    
    if(ids[position].instance_id == id_to_find.instance_id)
    {
      *out_index = position;
      return true;
    }
    
    if(ids[position].instance_id < id_to_find.instance_id)
    {
      down = position + 1;
    }
    else if(ids[position].instance_id > id_to_find.instance_id)
    {
      up = position - 1;
    }
  }
  
  assert(false);
  return false;
}


bool
find_index_linearly(uint32_t *out_index,
                    const Entity_id id_to_find,
                    const Entity_id ids[],
                    const uint32_t number_of_entities)
{
  assert(out_index);
  
  // TODO: Speed here to use simd.
  
  for(uint32_t i = 0; i < number_of_entities; ++i)
  {
    if(ids[i] == id_to_find)
    {
      (*out_index) = i;
      return true;
    }
  }
  
  return false;
}


} // ns
} // ns