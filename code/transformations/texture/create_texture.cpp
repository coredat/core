#include <transformations/texture/create_texture.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <3rdparty/stb/stb_image.h>


namespace Texture_utils {


void
create_texture_from_file(const char *filepath, Ogl::Texture *texture)
{
  // Create texture
  int width, height, n;
  
  unsigned char *img = stbi_load(filepath, &width, &height, &n, 4);

  Ogl::texture_create_2d(texture, width, height, GL_RGBA, true, img);
  
  stbi_image_free(img);
}


} // ns