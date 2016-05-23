#include <data/world_data/entity_data.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace
{
  constexpr uint32_t entity_data_size_of_name = 32;
  constexpr uint32_t entity_data_max_entities = 2048; // temp.
}


namespace World_data {


void
lock(Entity_data *data)
{
}


void
unlock(Entity_data *data)
{
}


void
entity_data_init(Entity_data *data,
                 const uint32_t size_hint)
{
  assert(data);

  lock(data);
  
  LOG_TODO("Use memory pool");
  
  static util::generic_id ids[entity_data_max_entities];
  #ifndef NDEBUG
  memset(ids, 0, sizeof(ids));
  #endif
  data->entity_id = ids;
  
  static char name_ptrs[entity_data_max_entities];
  #ifndef NDEBUG
  memset(name_ptrs, 0, sizeof(name_ptrs));
  #endif
  data->entity_name = name_ptrs;
  
  static uint32_t tags[entity_data_max_entities];
  #ifndef NDEBUG
  memset(tags, 0, sizeof(tags));
  #endif
  data->tags = tags;
  
  static uintptr_t user_data[entity_data_max_entities];
  #ifndef NDEBUG
  memset(user_data, 0, sizeof(user_data));
  #endif
  data->user_data = user_data;
  
  uint32_t *capacity = const_cast<uint32_t*>(&data->capacity);
  *capacity = entity_data_max_entities;
  
  unlock(data);
}


void
entity_data_free(Entity_data *data)
{
  lock(data);
  unlock(data);
}


void
entity_data_add_entity(Entity_data *data,
                       const util::generic_id id,
                       const char *name,
                       const uint32_t *tags)
{
  assert(data && data->size < data->capacity);
  assert(id);

  lock(data);
  
  const uint32_t index = data->size;
  ++(data->size);
  
  data->entity_id[index] = id;
  
  if(tags) { data->tags[index] = *tags; }

  if(name)
  {
    strlcpy(&data->entity_name[index * entity_data_size_of_name], name, entity_data_size_of_name);
  }
  else
  {
    data->entity_name[index * entity_data_size_of_name] = '\0';
  }

  unlock(data);
}


void
entity_data_set_name(Entity_data *data,
                     const util::generic_id id,
                     const char *name)
{
  assert(data && id && name);

  lock(data);

  size_t index;
  
  if(entity_data_exists(data, id, &index))
  {
    char *dest = &data->entity_name[index * entity_data_size_of_name];
    strncpy(dest, name, entity_data_size_of_name - 1);
  }
  else
  {
    LOG_WARNING("Entity id wasn't found");
  }
  
  unlock(data);
}
  
  
char*
entity_data_get_name(Entity_data *data,
                     const util::generic_id id)
{
  assert(data && id);
  
  lock(data);
  
  size_t index;
  char *result = nullptr;
  
  if(entity_data_exists(data, id, &index))
  {
    result = &data->entity_name[index * entity_data_size_of_name];
  }
  else
  {
    LOG_WARNING("Entity id wasn't found");
  }
  
  unlock(data);
  
  return result;
}


void
entity_data_remove_entity(Entity_data *data,
                          const util::generic_id id)
{
  assert(id && data);
  
  lock(data);
  
  size_t index_to_erase;
  if(entity_data_exists(data, id, &index_to_erase))
  {
    const size_t start_index = index_to_erase + 1;
    const size_t size_to_end = data->size - index_to_erase - 1;
    --(data->size);
    
    memmove(&data->entity_id[index_to_erase],   &data->entity_id[start_index],    size_to_end * sizeof(*data->entity_id));
    memmove(&data->entity_name[index_to_erase * entity_data_size_of_name], &data->entity_name[start_index * entity_data_size_of_name],  size_to_end * entity_data_size_of_name * sizeof(*data->entity_name));
    memmove(&data->user_data[index_to_erase],   &data->user_data[start_index],    size_to_end * sizeof(*data->user_data));
    memmove(&data->tags[index_to_erase],        &data->tags[start_index],         size_to_end * sizeof(*data->tags));
  }
  else
  {
    LOG_ERROR("Tried removing an entity that doesn't exist");
    assert(false);
  }
  
  unlock(data);
}


bool
entity_data_exists(Entity_data *data,
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