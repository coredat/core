#include "texture_manager.hpp"
#include <algorithm>
#include <SOIL/SOIL.h>
#include <assert.h>
#include <renderer/graphics_api/ogl/ogl_texture.hpp>



namespace Texture {


void
texture_load(const Load_texture textures_to_load[],
             const std::size_t number_of_textures_to_load,
             renderer::texture output[],
             const std::size_t size_of_output_pool)
{
  const std::size_t number_to_load = std::min(number_of_textures_to_load, size_of_output_pool);

  for(std::size_t i = 0; i < number_to_load; ++i)
  {
    const auto &tex = textures_to_load[i];
    
    // If our output index is bigger then the index
    if(tex.id > size_of_output_pool)
    {
      assert(false);
      continue;
    }
  
    int32_t width = 0;
    int32_t height = 0;
    
    uint8_t *image_data = SOIL_load_image(tex.file_path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    output[tex.id] = renderer::texture(image_data, width, height);
    
//    Ogl::Texture new_texture;
//    Ogl::texture_create_2d(&new_texture, width, height, Ogl::Pixel_format::rgba8, image_data);
//    
//    output[tex.id] = new_texture;
    
    SOIL_free_image_data(image_data);
  }
}


} // ns