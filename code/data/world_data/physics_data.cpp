#include "physics_data.hpp"
#include <utilities/logging.hpp>


namespace World_data {


void
physics_init(Physics_data *data, const uint32_t size)
{
  LOG_TODO("get rid of static data");

  static util::generic_id entity_data[2048];
  data->entity_id = entity_data;
  memset(entity_data, 0, sizeof(entity_data));
  
  static math::transform trans[2048];
  data->transform = trans;
  memset(trans, 0, sizeof(trans));
  
  static math::aabb aabbs[2048];
  data->aabb_collider = aabbs;
  
  data->capacity = 2048;
}


void
physics_free(Physics_data *data)
{
  // Delete the contents.
}


void
physics_add(Physics_data *data,
            const util::generic_id id,
            const math::aabb *aabb,
            const math::transform *trans)
{
  uint32_t index = -1;
  
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    // Found a free slot
    if(i < index && data->entity_id[i] == util::generic_id_invalid())
    {
      index = i;
    }
    
    // Found an existing entity that shares same id.
    if(data->entity_id[i] == id)
    {
      index = i;
      break;
    }
  }
  
  if(index < (uint32_t)-1)
  {
    data->entity_id[index] = id;
    data->aabb_collider[index] = *aabb;
    data->transform[index] = *trans;
  }
  else
  {
    LOG_ERROR("Failed to find a free slot");
  }
  
  ++data->size;
}


void
physics_remove(Physics_data *data,
               const util::generic_id id)
{
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    if(data->entity_id[i] == id)
    {
      const uint32_t start = i;
      
      const uint32_t start_move = i + 1;
      const uint32_t end_move = data->capacity - i - 1;
      
      memmove(&data->entity_id[start], &data->entity_id[start_move], end_move * sizeof(*data->entity_id));
      memmove(&data->transform[start], &data->transform[start_move], end_move * sizeof(*data->transform));
      memmove(&data->aabb_collider[start], &data->aabb_collider[start_move], end_move * sizeof(*data->aabb_collider));
      
      --data->size;
    }
  }
}


void
physics_update(Physics_data *data,
               const util::generic_id id,
               const math::aabb *aabb,
               const math::transform *trans)
{
  for(uint32_t i = 0; i < data->capacity; ++i)
  {
    // Found an existing entity that shares same id.
    if(data->entity_id[i] == id)
    {
      data->aabb_collider[i] = *aabb;
      data->transform[i] = *trans;
    }
  }
}



} // ns