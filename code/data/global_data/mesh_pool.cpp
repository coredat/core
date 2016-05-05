#include "mesh_pool.hpp"
#include <utilities/logging.hpp>
#include <utilities/obj_model_loader.hpp>
#include <math/geometry/aabb.hpp>

#ifdef _WIN32
#include <atomic>
using std::atomic_uint_least32_t;
#else
#include <stdatomic.h>
#endif


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


Graphics_api::Mesh
mesh_pool_find(const Mesh_pool *pool, const uint32_t id)
{
  assert(pool);

  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == id)
    {
      return pool->mesh[i];
    }
  }
  
  LOG_ERROR("Couldn't find resources for requested id")
  
  return Graphics_api::Mesh();
}


bool
mesh_pool_find_id_by_name(const Mesh_pool *pool,
                          const char *name,
                          uint32_t *result)
{
  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    const uint32_t str_offset = i * MESH_POOL_MAX_FILEPATH_SIZE;
    
    if(strcmp(name, &pool->filename[str_offset]) == 0)
    {
      *result = pool->id[i];
      return true;
    }
  }
  
  return false;
}



uint32_t
mesh_pool_push_new(Mesh_pool *pool,
                   const char *key,
                   const float *positions,
                   const float *normals,
                   const float *uvs,
                   const uint32_t number_of_vertices,
                   const uint32_t *index_data,
                   const uint32_t number_of_indices)
{
  // Search for key (todo)
  uint32_t search_index(0);
  if(mesh_pool_find_id_by_name(pool, key, &search_index))
  {
    return search_index;
  }
  
  // search for a zero
  // TODO: Has no mechanism to fail.
  uint32_t free_index(0);
  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == 0)
    {
      free_index = i;
      break;
    }
  }
  
  // Create mesh format.
  std::vector<float> pos(positions, positions + number_of_vertices);
  std::vector<float> norm(normals, normals + number_of_vertices);
  std::vector<float> tex_c(uvs, uvs + number_of_vertices);
  std::vector<uint32_t> ind(index_data, index_data + number_of_indices);
  const util::gl_mesh imported_mesh = util::create_open_gl_mesh(pos, tex_c, norm, ind);

  // Create graphics mesh and insert it.
  {
    Graphics_api::Mesh mesh;
    Graphics_api::mesh_create_new(&mesh,
                                  imported_mesh.mesh_data.data(),
                                  imported_mesh.mesh_data.size(),
                                  nullptr,
                                  0);
    
    const uint32_t new_id = ++mesh_id_counter;
    
    pool->id[free_index] = new_id;
    pool->mesh[free_index] = mesh;
    pool->aabb[free_index] = math::aabb_init_from_xyz_data(positions, number_of_vertices);

    // string
    const uint32_t str_offset = free_index * MESH_POOL_MAX_FILEPATH_SIZE;
    memset(&pool->filename[str_offset], 0, sizeof(char) * MESH_POOL_MAX_FILEPATH_SIZE);
    memcpy(&pool->filename[str_offset], key, sizeof(char) * strlen(key));
    
    return new_id;
  }

  return 0;
}


} // ns