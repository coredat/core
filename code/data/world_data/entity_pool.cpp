#include "entity_pool.hpp"
#include <assert.h>
#include <stddef.h>
#include <vector>
#include <core/memory/memory.hpp>
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
    const Core::Memory::Chunk id_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(Core::Entity_id));
    pool->entity_id = static_cast<Core::Entity_id*>(id_chunk.start_of_chunk);
    memset(pool->entity_id, 0, id_chunk.bytes_in_chunk);
  }
  
  {
    const Core::Memory::Chunk parent_id_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(Core::Entity_id));
    pool->parent_id = static_cast<Core::Entity_id*>(parent_id_chunk.start_of_chunk);
    memset(pool->parent_id, 0, parent_id_chunk.bytes_in_chunk);
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
    const Core::Memory::Chunk model_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(Resource::Model::ENUM));
    pool->model = static_cast<Resource::Model::ENUM*>(model_chunk.start_of_chunk);
    memset(pool->model, 0, model_chunk.bytes_in_chunk);
  }
  
  {
    const Core::Memory::Chunk texture_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(Resource::Texture::ENUM));
    pool->texture = static_cast<Resource::Texture::ENUM*>(texture_chunk.start_of_chunk);
    memset(pool->texture, 0, texture_chunk.bytes_in_chunk);
  }
  
  {
    const Core::Memory::Chunk transform_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(math::transform));
    pool->transform = static_cast<math::transform*>(transform_chunk.start_of_chunk);
    memset(pool->transform, 0, transform_chunk.bytes_in_chunk);
  }
  
  {
    const Core::Memory::Chunk aabb_chunk = Core::Memory::request_chunk(ENTITY_POOL_SIZE * sizeof(math::aabb));
    pool->aabb = static_cast<math::aabb*>(aabb_chunk.start_of_chunk);
    memset(pool->aabb, 0, aabb_chunk.bytes_in_chunk);
  }
}


void
entity_pool_de_init(Entity_pool *pool)
{
  assert(pool);
  
  for(uint32_t i = 0; i < pool->size; ++i)
  {
    if(pool->entity_id[i] != Core::Entity_id_util::invalid_id())
    {
      // TODO: Re add this when more scene graph things in.
      //assert(false);
    }
  }
}


bool
entity_pool_push_new_entity(Entity_pool *pool, const Core::Entity_id id)
{
  if(pool->size < pool->capacity)
  {
    pool->entity_id[pool->size] = id;
    pool->parent_id[pool->size] = Core::Entity_id_util::invalid_id();
    pool->entity_properties[pool->size] = Entity_properties{0};
    pool->name[pool->size * max_entity_name_size] = '\0';
    pool->model[pool->size] = (Resource::Model::ENUM)0;
    pool->texture[pool->size] = (Resource::Texture::ENUM)0;
    pool->aabb[pool->size] = math::aabb();
    
    ++(pool->size);
    
    return true;
  }

  // Full capacity.
  assert(false);
  return false;
}


bool
entity_pool_remove_entity(Entity_pool *pool, const Core::Entity_id id)
{
  // We move down all the elements in the data down one.
  // This way we can keep fragmentation and cache misses out
  // when processing the data, but take a hit here.1
  uint32_t remove_id;
  if(Core::Entity_id_util::find_index_linearly(&remove_id,
                                               id,
                                               pool->entity_id,
                                               pool->size))
  {
    const uint32_t start_move = remove_id + 1;
    const uint32_t end_move = pool->size - remove_id - 1;
  
    memmove(&pool->entity_id[remove_id],          &pool->entity_id[start_move],          end_move * sizeof(*pool->entity_id));
    memmove(&pool->parent_id[remove_id],          &pool->parent_id[start_move],          end_move * sizeof(*pool->parent_id));
    memmove(&pool->entity_properties[remove_id],  &pool->entity_properties[start_move],  end_move * sizeof(*pool->entity_properties));
    memmove(&pool->name[remove_id + max_entity_name_size],  &pool->entity_properties[start_move + max_entity_name_size],  end_move * sizeof(*pool->name));
    memmove(&pool->model[remove_id],              &pool->model[start_move],              end_move * sizeof(*pool->model));
    memmove(&pool->texture[remove_id],            &pool->texture[start_move],            end_move * sizeof(*pool->texture));
    memmove(&pool->transform[remove_id],          &pool->transform[start_move],          end_move * sizeof(*pool->transform));
    memmove(&pool->aabb[remove_id],               &pool->aabb[start_move],               end_move * sizeof(*pool->aabb));
    
    --(pool->size);
    
    return true;
  }

  assert(false); // uhuo
  return false;
}


const char *
entity_pool_get_entity_name(const Entity_pool *pool, const Core::Entity_id id)
{
  assert(pool);
  assert(id != Core::Entity_id_util::invalid_id());
  
  uint32_t remove_id(0);
  if(Core::Entity_id_util::find_index_linearly(&remove_id,
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
                            const Core::Entity_id id,
                            const char *set_name)
{
  assert(pool);
  assert(id != Core::Entity_id_util::invalid_id());
  
  uint32_t set_id(0);
  if(Core::Entity_id_util::find_index_linearly(&set_id,
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


} // ns