#include "entity_data.hpp"
#include <assert.h>


namespace Data {


bool
entity_get_index(const Entity entities[], const std::size_t number_of_entities, const Entity_id id, std::size_t *index)
{
  // Param check
  assert(index && number_of_entities && entities);
  
  for(std::size_t e = 0; e < number_of_entities; ++e)
  {
    if(entities->entity_id[e] == id)
    {
      *index = e;
      return true;
    }
  }
  
  assert(false); // oops didn't find an entity.
  return false;
}


void
entity_set_transform(Entity entities[],
                     const std::size_t number_of_entities,
                     const Entity_id id,
                     math::transform *set_transform)
{
  assert(set_transform);
  
  for(std::size_t e = 0; e < number_of_entities; ++e)
  {
    if(entities->entity_id[e] == id)
    {
      entities->transform[e] = *set_transform;
      return;
    }
  }
  assert(false);
}



void
entity_get_transform(const Entity entities[],
                     const std::size_t number_of_entities,
                     const Entity_id id,
                     math::transform *get_transform)
{
  for(std::size_t e = 0; e < number_of_entities; ++e)
  {
    if(entities->entity_id[e] == id)
    {
      *get_transform = entities->transform[e];
      return;
    }
  }
  assert(false);
}


}