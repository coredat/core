#ifndef TEXTURE_POOL_INCLUDED_6CFB6B90_56F8_408D_B068_9EB867B41233
#define TEXTURE_POOL_INCLUDED_6CFB6B90_56F8_408D_B068_9EB867B41233


#include <simple_renderer/texture.hpp>
#include <cstddef>


#define SIZE_OF_TEXTURE_POOL 64


namespace Data {


struct Texture_pool
{
  std::size_t         ids[SIZE_OF_TEXTURE_POOL];
  renderer::texture   textures[SIZE_OF_TEXTURE_POOL];
  
  const std::size_t   size = SIZE_OF_TEXTURE_POOL;
}; // struct


} // ns


#endif // inc guard