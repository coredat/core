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
  memset(ids, 0, sizeof(ids));
  data->entity_id = ids;
  
  static char *name_ptrs[entity_data_max_entities];
  memset(name_ptrs, 0, sizeof(name_ptrs));
  data->entity_name = name_ptrs;
  
  static char name_buffer[entity_data_max_entities * entity_data_size_of_name];
  memset(name_buffer, 0, sizeof(name_buffer));
  data->entity_name_buffer = name_buffer;
  
  static uint32_t tags[entity_data_max_entities];
  memset(tags, 0, sizeof(tags));
  data->tags = tags;
  
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
                       const util::generic_id id)
{
  lock(data);
  unlock(data);
}


void
entity_data_remove_entity(Entity_data *data,
                          const util::generic_id id)
{
  lock(data);
  unlock(data);
}


bool
entity_data_exists(Entity_data *data,
                   const util::generic_id id)
{
  lock(data);
  unlock(data);
  
  return false;
}


} // ns