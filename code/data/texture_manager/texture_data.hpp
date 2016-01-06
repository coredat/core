#ifndef TEXTURE_DATA_1A635A9D_89DC_4A7A_8F5A_9770961D64C8
#define TEXTURE_DATA_1A635A9D_89DC_4A7A_8F5A_9770961D64C8


#include <cstddef>
#include <stdint.h>
#include <simple_renderer/texture.hpp>
#include <string>


#define TEXTURE_BUFFER_SIZE 128


namespace Data {


struct Texture
{
  uint32_t          id[TEXTURE_BUFFER_SIZE];
  renderer::texture tex[TEXTURE_BUFFER_SIZE];
  
  const std::size_t size = TEXTURE_BUFFER_SIZE;
};


void          texture_init_data(Texture *data, const std::size_t number_of_textures);
bool          texture_is_valid(const Texture *data, const std::size_t texture_id);
uint32_t      texture_get_api_id(const Texture *data, const std::size_t texture_id);
std::size_t   texture_add_new(Texture *data, const std::size_t number_of_textures, const char* filepath, const uint32_t id);
std::size_t   texture_find_index(const Texture *data, const std::size_t number_of_textures, const uint32_t id);


} // ns


namespace Texture {


struct Load_texture
{
  std::size_t id;
  std::string file_path;
};


void
texture_init(const Load_texture textures_to_load[],
             const std::size_t number_of_textures_to_load,
             renderer::texture output[],
             const std::size_t size_of_output_pool);



} // ns


#endif // inc guard