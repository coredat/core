#include "texture_pool.hpp"
#include <assert.h>
#include <utilities/logging.hpp>
#include <SOIL/SOIL.h>

#ifdef _WIN32
#include <atomic>
using std::atomic_uint_least32_t;
#else
#include <stdatomic.h>
#endif


namespace
{
  atomic_uint_least32_t texture_id(0);
}


namespace Resource_data {


void
texture_pool_init(Texture_pool *pool)
{
  memset(pool->id, 0, sizeof(pool->id));
  memset(pool->texture, 0, sizeof(pool->texture));
  memset(pool->filepath, 0, sizeof(pool->filepath));
}


Ogl::Texture
texture_pool_find(const Texture_pool *pool, const uint32_t id)
{
  assert(pool);

  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == id)
    {
      return pool->texture[i];
    }
  }
  
  LOG_ERROR("Couldn't find resource for requested id")
  
  return Ogl::Texture();
}


uint32_t
texture_pool_load(Texture_pool *pool,
                  const char *filepath)
{
  // Check if filepath has already been loaded.
  uint32_t result_id;
  if(texture_pool_find_id_by_name(pool, filepath, &result_id))
  {
    return result_id;
  }

  // Create texture
  int width, height;
  
  unsigned char *img = SOIL_load_image(filepath,
                                       &width,
                                       &height,
                                       0,
                                       SOIL_LOAD_RGBA);
  
  // Add to pool
  const uint32_t texture_id = Resource_data::texture_pool_add(pool,
                                                              img,
                                                              width,
                                                              height,
                                                              0,
                                                              filepath);
  
  SOIL_free_image_data(img);
  
  return texture_id;
}


bool
texture_pool_find_id_by_name(const Texture_pool *pool,
                             const char *name,
                             uint32_t *result)
{
  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    const uint32_t str_offset = i * TEXTURE_POOL_MAX_FILEPATH_SIZE;
    
    if(strcmp(name, &pool->filepath[str_offset]) == 0)
    {
      *result = pool->id[i];
      return true;
    }
  }
  
  return false;
}


uint32_t
texture_pool_add(Texture_pool *pool,
                 const void *data,
                 const uint32_t width,
                 const uint32_t height,
                 const uint32_t depth,
                 const char* name)
{
  uint32_t index = 0;

  // Find a free index.
  for(uint32_t i = 0; i < pool->capacity; ++i)
  {
    if(pool->id[i] == 0)
    {
      index = i;
      break;
    }
    
    // Error
    if(i == pool->capacity - 1)
    {
      LOG_ERROR("Failed to find a free slot")
      return 0;
    }
  }

  Ogl::Texture new_texture;
  Ogl::texture_create_2d(&new_texture, width, height, GL_RGBA, data);
  
  if(Ogl::texture_is_valid(&new_texture))
  {
    ++texture_id;
    pool->id[index] = texture_id;
    pool->texture[index] = new_texture;
    
    const uint32_t str_offset = index * TEXTURE_POOL_MAX_FILEPATH_SIZE;
    memset(&pool->filepath[str_offset], 0, sizeof(char) * TEXTURE_POOL_MAX_FILEPATH_SIZE);
    memcpy(&pool->filepath[str_offset], name, sizeof(char) * strlen(name));
  }
  else
  {
    LOG_ERROR("Failed to create a texture")
    return 0;
  }
  
  return texture_id;
}


} // ns