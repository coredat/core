#include "ogl_texture.hpp"
#include "ogl_pixel_format.hpp"


namespace Ogl {


void
texture_create_2d(Texture *out_texture,
                  const uint32_t width,
                  const uint32_t height,
                  const GLenum format,
                  const void *data)
{
  // Param check
  if(!out_texture || !width || !height)
  {
    assert(false); // Param fail

    #ifdef OGL_EXTRA_PARAM_CHECKS
    if(!width || !height)
    {
      LOG_GL_PARAM_ERROR("Width and Height need to be greater than zero.");
    }
    
    if(!out_texture)
    {
      LOG_GL_PARAM_ERROR("Output texture cannot be null.");
    }
    
    if(!data)
    {
      LOG_GL_PARAM_ERROR("'data' may not be null.");
    }
    #endif
  
    return;
  }
  
  // Set some arguments
  out_texture->format    = format;
  out_texture->width     = width;
  out_texture->height    = height;
  out_texture->dimention = GL_TEXTURE_2D;
  
  const auto pixel_format      = pixel_format_get_format(format);
  const auto pixel_format_type = pixel_format_get_type(format);
  
  // Open GL
  glGenTextures(1, &out_texture->texture_id);
  glBindTexture(GL_TEXTURE_2D, out_texture->texture_id);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               format,
               width,
               height,
               0,
               pixel_format,
               pixel_format_type,
               data);
  
  glGenerateMipmap(GL_TEXTURE_2D);
}



void
texture_update_texture_2d(Texture *update_texture,
                          const uint32_t offset_x,
                          const uint32_t offset_y,
                          const uint32_t width,
                          const uint32_t height,
                          const void *data)
{
  // Param check
  if(!update_texture || !data || (width + offset_x) > update_texture->width || (height + offset_y) > update_texture->height)
  {
    assert(false); // Param fail
    
    #ifdef OGL_EXTRA_PARAM_CHECKS
    if(!update_texture)
    {
      LOG_GL_PARAM_ERROR("'update_texture' may not be null.");
    }
    
    if(!data)
    {
      LOG_GL_PARAM_ERROR("'data' may not be null.");
    }
    
    if((width + offset_x) > update_texture->width || (height + offset_y) > update_texture->height)
    {
      LOG_GL_PARAM_ERROR("Can't update an area larger than the target texture.\n");
    }
    #endif
    
    return;
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
  
  // Calling code checks for error.
}



} // ns