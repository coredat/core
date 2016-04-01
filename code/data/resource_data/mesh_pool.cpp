#include "mesh_pool.hpp"
#include <stdatomic.h>


namespace
{
  atomic_uint_least32_t mesh_id_counter(0);
  
  
  uint64_t
  hash(const char *str)
  {
    // http://www.cse.yorku.ca/~oz/hash.html
  
    uint64_t hash = 5381;
    int c;

    while (!!(c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
  }
}


namespace Resource_data {


void
mesh_pool_init(Mesh_pool *pool)
{
  memset(pool->id, 0, sizeof(pool->id));
  memset(pool->filename, 0, sizeof(pool->filename));
  memset(pool->mesh, 0, sizeof(pool->mesh));
  memset(pool->aabb, 0, sizeof(pool->aabb));
}


uint32_t
mesh_pool_push_new(Mesh_pool *pool,
                   const char *key,
                   const float *vertex_data,
                   const uint32_t number_of_vertices,
                   const uint32_t *index_data,
                   const uint32_t number_of_indices)
{
  // Search for key (todo)
  
  
  // search for a zero
  // TODO: Has no mechanism to fail.
  uint32_t free_index(0);
  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == 0)
    {
      free_index = 0;
      break;
    }
  }
  
  // Create graphics mesh and insert it.
  {
    Graphics_api::Mesh mesh;
    Graphics_api::mesh_create_new(&mesh,
                                  vertex_data,
                                  number_of_vertices,
                                  index_data,
                                  number_of_indices);
  }

  return ++mesh_id_counter;
}


} // ns