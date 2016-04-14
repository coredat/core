#ifndef TEXTURE_POOL_INCLUDED_6CFB6B90_56F8_408D_B068_9EB867B41233
#define TEXTURE_POOL_INCLUDED_6CFB6B90_56F8_408D_B068_9EB867B41233


#include <graphics_api/ogl/ogl_texture.hpp> // TODO: This can be fwd dec
#include <stdint.h>


#define SIZE_OF_TEXTURE_POOL 64
#define TEXTURE_POOL_MAX_FILEPATH_SIZE 256


namespace Resource_data {


struct Texture_pool
{
  uint32_t              id[SIZE_OF_TEXTURE_POOL];
  Ogl::Texture          texture[SIZE_OF_TEXTURE_POOL];
  char                  filepath[SIZE_OF_TEXTURE_POOL * TEXTURE_POOL_MAX_FILEPATH_SIZE];
  
  const uint32_t        capacity = SIZE_OF_TEXTURE_POOL;
}; // struct


/*!
  Initializes the pool, such that the ids will be in sequence.
  and the contents are zero.
*/
void
texture_pool_init(Texture_pool *pool);


/*!
  Find the texture for the given id.
*/
Ogl::Texture
texture_pool_find(const Texture_pool *pool, const uint32_t id);


/*!
  Find a texture by filepath.
*/
bool
texture_pool_find_id_by_name(const Texture_pool *pool,
                             const char *name,
                             uint32_t *result);


/*
  Adds a texture to the pool via a filepath.
*/
uint32_t
texture_pool_load(Texture_pool *pool,
                  const char *filepath);


/*!
  Add texture data to the pool
*/
uint32_t
texture_pool_add(Texture_pool *pool,
                 const void *data,
                 const uint32_t height,
                 const uint32_t width,
                 const uint32_t depth,
                 const char *name);


} // ns


#endif // inc guard