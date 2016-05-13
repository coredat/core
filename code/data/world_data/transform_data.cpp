#include <data/world_data/transform_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace
{
  constexpr uint32_t transform_max_entities = 2048; // temp.
}


namespace World_data {


void
lock(Transform_data *data)
{
}


void
unlock(Transform_data *data)
{
}


void
transform_data_init(Transform_data *data,
                    const uint32_t size_hint)
{
  lock(data);

  LOG_TODO("Use a memory pool");
  
  static util::generic_id ids[transform_max_entities];
  memset(ids, 0, sizeof(ids));
  data->entity_id = ids;
  
  static math::transform transforms[transform_max_entities];
  memset(transforms, 0, sizeof(transforms));
  data->transform = transforms;
  
  static math::aabb aabb[transform_max_entities];
  memset(aabb, 0, sizeof(aabb));
  data->aabb = aabb;
  
  uint32_t *capacity_bump = const_cast<uint32_t*>(&data->capacity);
  *capacity_bump = transform_max_entities;
  
  unlock(data);
}


void
transform_data_free(Transform_data *data)
{
}


void
transform_data_add_transform(Transform_data *data,
                             const util::generic_id id,
                             math::transform *trans,
                             math::aabb *aabb)
{
  assert(data && data->size < data->capacity);
  assert(id);

  lock(data);
  
  const uint32_t index = data->size;
  ++(data->size);
  
  data->entity_id[index] = id;
  
  if(trans) { data->transform[index] = *trans; }
  if(aabb)  { data->aabb[index] = *aabb;       }
  
  unlock(data);
}


void
transform_data_remove_transform(Transform_data *data,
                                const util::generic_id id)
{
  assert(id && data);
  
  lock(data);
  
  size_t index_to_erase;
  if(transform_data_exists(data, id, &index_to_erase))
  {
    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;
    --(data->size);
    
    memmove(&data->entity_id[index_to_erase], &data->entity_id[start_index], size_to_end * sizeof(*data->entity_id));
    memmove(&data->transform[index_to_erase], &data->transform[start_index], size_to_end * sizeof(*data->transform));
    memmove(&data->aabb[index_to_erase],      &data->aabb[start_index], size_to_end *      sizeof(*data->aabb));
  }
  else
  {
    assert(false);
    LOG_ERROR(Error_string::entity_not_found());    
  }
  
  unlock(data);
}


bool
transform_data_exists(Transform_data *data,
                      const util::generic_id id,
                      size_t *out_index)
{
  assert(data && id);
  
  bool found = false;
  
  size_t no_index;
  if(!out_index) { out_index = &no_index; }
  
  found = util::generic_id_search_binary(out_index, id, data->entity_id, data->size);
  
  return found;
}


} // ns