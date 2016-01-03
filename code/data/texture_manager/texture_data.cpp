#include "texture_data.hpp"
#include <SOIL/SOIL.h>
#include <assert.h>


namespace Data {


void
texture_init_data(Texture *data, const std::size_t number_of_textures)
{
  // Could just memset?
  for(std::size_t i = 0; i < number_of_textures; ++i)
  {
    // Uhuh
    data->id[i] = 0;
  }
}


bool
texture_is_valid(const Texture *data, const std::size_t texture_id)
{
  // DANGER!
  assert(data);
  return data->tex[texture_id].is_valid() > 0;
}


uint32_t
texture_get_api_id(const Texture *data, const std::size_t texture_id)
{
  assert(data && texture_is_valid(data, texture_id));
  return data->tex[texture_id].get_gl_id();
}


std::size_t
texture_add_new(Texture *data,
                const std::size_t number_of_textures,
                const char *filepath,
                const uint32_t id)
{
  // Danger!
  
  // Could just memset?
  for(std::size_t i = 0; i < number_of_textures; ++i)
  {
    if(data->tex[i].get_gl_id() == 0)
    {
      int32_t width = 0;
      int32_t height = 0;
      
      uint8_t *image_data = SOIL_load_image(filepath, &width, &height, 0, SOIL_LOAD_RGBA);

      renderer::texture texture;
      texture.load_data(image_data, width, height);
      SOIL_free_image_data(image_data);
      
      data->tex[i] = texture;
      data->id[i] = id;
      
      return i;
    }
  }
  
  return 0;
}


std::size_t
texture_find_index(const Texture *data, const std::size_t number_of_textures, const uint32_t id)
{
  for(std::size_t i = 0; i < number_of_textures; ++i)
  {
    if(data->id[i] == id)
    {
      return i;
    }
  }
  
  return 0; // Errr 0 is valid.
}


} // ns