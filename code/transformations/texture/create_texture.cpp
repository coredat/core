#include <transformations/texture/create_texture.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <SOIL/SOIL.h>


namespace Texture_utils {


void
create_texture_from_file(const char *filepath, Ogl::Texture *texture)
{
  // Create texture
  int width, height;
  
  unsigned char *img = SOIL_load_image(filepath,
                                       &width,
                                       &height,
                                       0,
                                       SOIL_LOAD_RGBA);

  Ogl::texture_create_2d(texture, width, height, GL_RGBA, img);
  
  SOIL_free_image_data(img);
}


} // ns