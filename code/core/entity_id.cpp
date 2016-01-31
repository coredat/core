#include "entity_id.hpp"
#include <assert.h>
#include <cstring>
#include <math/math.hpp>


namespace Core {


void
init_to_invalid_ids(Entity_id ids[], const std::size_t size_of_entities)
{
  memset(ids, 0, size_of_entities * sizeof(Entity_id));
}


bool
find_index_linearly(std::size_t *out_index,
                    const Entity_id id_to_find,
                    const Entity_id ids[],
                    const std::size_t number_of_entities)
{
  assert(out_index);
  
  for(std::size_t i = 0; i < number_of_entities; ++i)
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