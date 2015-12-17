#ifndef TEXTURE_DATA_
#define TEXTURE_DATA_


#include <cstddef>
#include <stdint.h>
#include <simple_renderer/texture.hpp>


#define TEXTURE_BUFFER_SIZE 128


namespace Data {


struct Texture
{
  renderer::texture tex[TEXTURE_BUFFER_SIZE];
};


void          texture_init_data(Texture *data, const std::size_t number_of_textures);
bool          texture_is_valid(const Texture *data, const std::size_t texture_id);
uint32_t      texture_get_api_id(const Texture *data, const std::size_t texture_id);
std::size_t   texture_add_new(Texture *data, const std::size_t number_of_textures, const char* filepath);


} // ns


#endif // inc guard