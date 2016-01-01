#ifndef OGL_TEXTURE_INCLUDED_530B56CF_2269_430E_ACC6_04D56756ACC4
#define OGL_TEXTURE_INCLUDED_530B56CF_2269_430E_ACC6_04D56756ACC4


#include "ogl_common.hpp"
#include "ogl_pixel_format.hpp"
#include <stdint.h>
#include <assert.h>


namespace Ogl {


struct Texture
{
  GLuint          texture_id  = 0;
  uint32_t        width       = 0;
  uint32_t        height      = 0;
  Pixel_format    format      = Pixel_format::rgba8;
}; // struct


inline void
texture_create_2d(Texture *out_texture, const uint32_t width, const uint32_t height, const Pixel_format format, const void *data, std::ostream *log = nullptr)
{
  // Param check
  if(!out_texture || !width || !height || !data)
  {
    assert(false); // Param fail
  
    if(log)
    {
      (*log) << "Incorrect paramaters given to 'Ogl::texture_create_2d'.\n";
    }
  
    return;
  }
  
  // Set some arguments
  out_texture->format = format;
  out_texture->width = width;
  out_texture->height = height;
  
  // Open GL
  glGenTextures(1, &out_texture->texture_id);
  glBindTexture(GL_TEXTURE_2D, out_texture->texture_id);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               pixel_format_get_internal_format(format),
               width,
               height,
               0,
               pixel_format_get_format(format),
               pixel_format_get_type(format),
               data);
  
  error_check(log, "Error while creating a texture in 'Ogl::texture_create_2d'.");
}



inline void
texture_update_texture_2d(Texture *update_texture,
                          const uint32_t offset_x,
                          const uint32_t offset_y,
                          const uint32_t width,
                          const uint32_t height,
                          const void *data,
                          std::ostream *log = nullptr)
{
  // Param check
  if(!update_texture || !data || (width + offset_x) > update_texture->width || (height + offset_y) > update_texture->height)
  {
    assert(false); // Param fail
    
    if(log)
    {
      (*log) << "Incorrect paramaters given to Ogl::texture_update_texture_2d";
    }
  }
  
  glBindTexture(GL_TEXTURE_2D, update_texture->texture_id);
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  offset_x,
                  offset_y,
                  width,
                  height,
                  pixel_format_get_format(update_texture->format),
                  pixel_format_get_type(update_texture->format),
                  data);
}


} // ns


#endif // inc guard