#include "entity_pool.hpp"
#include <assert.h>
#include <stddef.h>
#include <vector>
#include <core/memory/memory.hpp>
#include <math/transform/transform.hpp>
#include <utilities/logging.hpp>
#include <utilities/memory.hpp>
#include <iostream>

namespace
{
  constexpr uint32_t max_entity_name_size = 32;
}


namespace World_data {


void
entity_pool_init(Entity_pool *pool)
{
  {
    const Core::Memory::Chunk id_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(util::generic_id));
    pool->entity_id = static_cast<util::generic_id*>(id_chunk.start_of_chunk);
    memset(pool->entity_id, 0, id_chunk.bytes_in_chunk);
  }
  
  {
    const Core::Memory::Chunk ent_prop_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(Entity_properties));
    pool->entity_properties = static_cast<Entity_properties*>(ent_prop_chunk.start_of_chunk);
    memset(pool->entity_properties, 0, ent_prop_chunk.bytes_in_chunk);
  }
  
  {
    const Core::Memory::Chunk ent_name_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * (sizeof(char) * max_entity_name_size));
    pool->name = static_cast<char*>(ent_name_chunk.start_of_chunk);
    memset(pool->name, 0, ent_name_chunk.bytes_in_chunk);
  }
  
  {
//    const Core::Memory::Chunk model_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(uint32_t));
//    pool->model = static_cast<uint32_t*>(model_chunk.start_of_chunk);
//    memset(pool->model, 0, model_chunk.bytes_in_chunk);
  }
  
  {
//    const Core::Memory::Chunk texture_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(uint32_t));
//    pool->texture = static_cast<uint32_t*>(texture_chunk.start_of_chunk);
//    memset(pool->texture, 0, texture_chunk.bytes_in_chunk);
  }
  
  {
    //const Core::Memory::Chunk transform_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(math::transform));
    //pool->transform = static_cast<math::transform*>(transform_chunk.start_of_chunk);
    //memset(pool->transform, 0, transform_chunk.bytes_in_chunk);
    
    LOG_TODO("Stack allocated transform pool, because we need memory 16 byte aligned.");
    
    static math::transform dummy_alloc[ENTITY_POOL_SIZE];
    pool->transform = dummy_alloc;
    
    for(uint32_t i = 0; i < ENTITY_POOL_SIZE; ++i)
    {
      pool->transform[i] = math::transform();
    }
  }
  
  {
    LOG_TODO("Stack allocated aabb pool, because we need memory 16 byte aligned.");
    
    //const Core::Memory::Chunk aabb_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(math::aabb));
    //pool->aabb = static_cast<math::aabb*>(aabb_chunk.start_of_chunk);
    //memset(pool->aabb, 0, aabb_chunk.bytes_in_chunk);
    
    static math::aabb dumm_alloc_2[ENTITY_POOL_SIZE];
    pool->aabb = dumm_alloc_2;
  }
}


void
entity_pool_de_init(Entity_pool *pool)
{
  assert(pool);
  
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->entity_id[i] != util::generic_id_invalid())
    {
      // TODO: Re add this when more scene graph things in.
      //assert(false);
    }
  }
}


bool
entity_pool_find_index(Entity_pool *pool, const util::generic_id id, size_t *out_index)
{
  if(util::generic_id_search_binary(out_index,
                                    id,
                                    pool->entity_id,
                                    pool->size))
  {
    return true;
  }
  
  return false;
}


bool
entity_pool_push_new_entity(Entity_pool *pool, const util::generic_id id)
{
  if(pool->size < pool->capacity)
  {
    const uint32_t index = pool->size;
    ++(pool->size);
  
    pool->entity_id[index] = id;
    pool->entity_properties[index] = Entity_properties{0};
    pool->name[index * max_entity_name_size] = '\0';
//    pool->model[index] = (uint32_t)0;
//    pool->texture[index] = (uint32_t)0;
    pool->aabb[index] = math::aabb();
    pool->transform[index] = math::transform_init(math::vec3_zero(), math::vec3_one(), math::quat_init());
    
    
    return true;
  }

  // Full capacity.
  assert(false);
  return false;
}


bool
entity_pool_remove_entity(Entity_pool *pool, const util::generic_id id)
{
  // We move down all the elements in the data down one.
  // This way we can keep fragmentation and cache misses out
  // when processing the data, but take a hit here.1
  size_t remove_id;
  if(util::generic_id_search_binary(&remove_id,
                                    id,
                                    pool->entity_id,
                                    pool->size))
  {
    const uint32_t start_move = remove_id + 1;
    const uint32_t end_move = pool->size - remove_id - 1;
    --(pool->size);
        
    memmove(&pool->entity_id[remove_id],          &pool->entity_id[start_move],          end_move * sizeof(*pool->entity_id));
    memmove(&pool->entity_properties[remove_id],  &pool->entity_properties[start_move],  end_move * sizeof(*pool->entity_properties));
    memmove(&pool->name[remove_id * max_entity_name_size],  &pool->name[start_move * max_entity_name_size],  end_move * (sizeof(char) * max_entity_name_size));
    memmove(&pool->transform[remove_id],          &pool->transform[start_move],          end_move * sizeof(*pool->transform));
    memmove(&pool->aabb[remove_id],               &pool->aabb[start_move],               end_move * sizeof(*pool->aabb));
//    memmove(&pool->model[remove_id],              &pool->model[start_move],              end_move * sizeof(*pool->model));
//    memmove(&pool->texture[remove_id],            &pool->texture[start_move],            end_move * sizeof(*pool->texture));
    
    
    return true;
  }

  assert(false); // uhuo
  return false;
}


char *
entity_pool_get_entity_name(const Entity_pool *pool, const util::generic_id id)
{
  assert(pool);
  assert(id != util::generic_id_invalid());
  
  size_t remove_id(0);
  if(util::generic_id_search_binary(&remove_id,
                                    id,
                                    pool->entity_id,
                                    pool->size))
  {
    return &pool->name[remove_id * max_entity_name_size];
  }
  
  assert(false); // A valid entity id should have got something.
  return nullptr; // Didn't find anything.
}


void
entity_pool_set_entity_name(const Entity_pool *pool,
                            const util::generic_id id,
                            const char *set_name)
{
  assert(pool);
  assert(id != util::generic_id_invalid());
  
  size_t set_id(0);
  if(util::generic_id_search_binary(&set_id,
                                    id,
                                    pool->entity_id,
                                    pool->size))
  {
    const uint32_t index = set_id * max_entity_name_size;
  
    strncpy(&pool->name[index],
            set_name,
            max_entity_name_size - 1);
    
    pool->name[index + max_entity_name_size] = '\0'; // Make sure last char is nullptr
  }
}


math::transform
entity_pool_get_transform(Entity_pool *pool,
                          const util::generic_id id)
{
  assert(pool);
  assert(id != util::generic_id_invalid());

  size_t find_index(0);
  if(util::generic_id_search_binary(&find_index,
                                    id,
                                    pool->entity_id,
                                    pool->size))
  {
    return pool->transform[find_index];
  }

  LOG_ERROR("Unable to find transform");
  return math::transform_init(math::vec3_zero(), math::vec3_one(), math::quat_init());
}


} // ns