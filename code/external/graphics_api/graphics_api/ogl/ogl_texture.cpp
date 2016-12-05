#include "ogl_texture.hpp"
#include "ogl_pixel_format.hpp"
#include "ogl_dimentions.hpp"
#include "../texture_desc.hpp"
#include "../pixel_format.hpp"


namespace Ogl {


void
texture_create(Graphics_api::Texture_desc *in_out_desc,
               const void *data)
{
  // Param
  assert(in_out_desc);
  
  /*
    Pull out the information from the description.
  */
  const GLenum internal_fmt = pixel_format_get_gl_internal_format(in_out_desc->pixel_format);
  const GLenum format       = pixel_format_get_format(internal_fmt);
  const GLenum type         = pixel_format_get_type(internal_fmt);
  const GLenum dimention    = dimention_to_gl_texture_dimention(in_out_desc->dimention);
  
  /*
    Generate the texture
  */
  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(dimention, texture_id);
  
  switch(dimention)
  {
    case(GL_TEXTURE_1D):
    {
      glTexImage1D(GL_TEXTURE_1D,
                   0,
                   internal_fmt,
                   in_out_desc->width,
                   0,
                   format,
                   type,
                   data);
      
      break;
    }
    
    case(GL_TEXTURE_2D):
    {
      glTexImage2D(GL_TEXTURE_2D,
             0,
             internal_fmt,
             in_out_desc->width,
             in_out_desc->height,
             0,
             format,
             type,
             data);
      
      break;
    }
    
    case(GL_TEXTURE_3D):
    {
      // Not implimented.
      assert(false);
    
      break;
    }
      
  }
  
  /*
    Generate Mips if requested.
  */
  if(in_out_desc->mips)
  {
    glGenerateMipmap(dimention);
  }
  
  /*
    Set some of the desc items.
  */
  in_out_desc->platform_handle = static_cast<uintptr_t>(texture_id);
  in_out_desc->pixel_format    = pixel_format_get_gfx_api_format(format);
  
  /*
    Remove any gl bindings.
  */
  glBindTexture(dimention, 0);
  
  /*
    Check state
  */
  #ifndef GL_EXTRA_ERROR_CHECKS
  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(err_code, "Error creating texture.");
  }
  #endif
}


void
texture_update(const Graphics_api::Texture_desc *desc,
               const uint32_t offset_x,
               const uint32_t offset_y,
               const uint32_t offset_z,
               const uint32_t width,
               const uint32_t height,
               const uint32_t depth,
               const void *data)
{
  assert(desc);
  
  const GLenum dimention    = dimention_to_gl_texture_dimention(desc->dimention);
  const GLenum internal_fmt = pixel_format_get_gl_internal_format(desc->pixel_format);
  const GLenum format       = pixel_format_get_format(internal_fmt);
  const GLenum type         = pixel_format_get_type(format);
  const GLuint texture_id   = static_cast<GLuint>(desc->platform_handle);

  if(format == GL_RED)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  
  glBindTexture(dimention, texture_id);
  
  switch(dimention)
  {
    case(GL_TEXTURE_1D):
    {
      glTexSubImage1D(GL_TEXTURE_1D,
                      0,
                      offset_x,
                      width,
                      format,
                      type,
                      data);
      break;
    }
    
    case(GL_TEXTURE_2D):
    {
      glTexSubImage2D(GL_TEXTURE_2D,
                      0,
                      offset_x,
                      offset_y,
                      width,
                      height,
                      format,
                      type,
                      data);
    
      break;
    }
    
    case(GL_TEXTURE_3D):
    {
      // Not implimented.
      assert(false);
    }
  }
  
  if(desc->mips)
  {
    glGenerateMipmap(dimention);
  }
  
  // Put packing back to what it was.
  if(format == GL_RED)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  
  /*
    Tidy up
  */
  glBindTexture(dimention, 0);
  
  /*
    Check state
  */
  #ifndef GL_EXTRA_ERROR_CHECKS
  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(err_code, "Error updating texture.");
  }
  #endif
}


void
texture_create_1d(Texture *out_texture,
                  const uint32_t width,
                  const GLenum format,
                  const void *data)
{
  // Param check
  assert(out_texture);
  assert(width);
  assert(format);
//  assert(data);
  
  // Set some arguments
  out_texture->texture_id = 0;
  out_texture->format     = format;
  out_texture->width      = width;
  out_texture->height     = 1;
  out_texture->depth      = 0;
  out_texture->dimention  = GL_TEXTURE_1D;
  out_texture->has_mips   = false;
  
  const auto pixel_format      = pixel_format_get_format(format);
  const auto pixel_format_type = pixel_format_get_type(format);
  
  // Open GL
  glGenTextures(1, &out_texture->texture_id);
  glBindTexture(GL_TEXTURE_1D, out_texture->texture_id);
  glTexImage1D(GL_TEXTURE_1D,
               0,
               format,
               width,
               0,
               pixel_format,
               pixel_format_type,
               data);
  
  if(out_texture->has_mips)
  {
    glGenerateMipmap(GL_TEXTURE_1D);
  }
}


void
texture_create_2d(Texture *out_texture,
                  const uint32_t width,
                  const uint32_t height,
                  const GLenum format,
                  const bool create_mips,
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
  out_texture->has_mips  = create_mips;
  
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
  
  if(create_mips)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}


void
texture_update_texture_1d(Texture *update_texture,
                          const uint32_t offset_x,
                          const uint32_t width,
                          const void *data)
{
  // Param Check
  assert(update_texture);
  assert(width);
  assert(data);
  
  if(update_texture->format == GL_RED)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  
  glBindTexture(GL_TEXTURE_1D, update_texture->texture_id);
  glTexSubImage1D(GL_TEXTURE_1D,
                  0,
                  offset_x,
                  width,
                  pixel_format_get_format(update_texture->format),
                  pixel_format_get_type(update_texture->format),
                  data);
  
  if(update_texture->has_mips)
  {
    glGenerateMipmap(GL_TEXTURE_1D);
  }
  
  // Put packing back to what it was.
  if(update_texture->format == GL_RED)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
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
  if(!update_texture || (width + offset_x) > update_texture->width || (height + offset_y) > update_texture->height)
  {
    assert(false); // Param fail
    
    #ifdef OGL_EXTRA_PARAM_CHECKS
    if(!update_texture)
    {
      LOG_GL_PARAM_ERROR("'update_texture' may not be null.");
    }
    
    if((width + offset_x) > update_texture->width || (height + offset_y) > update_texture->height)
    {
      LOG_GL_PARAM_ERROR("Can't update an area larger than the target texture.\n");
    }
    #endif
    
    return;
  }
  
  if(update_texture->format == GL_RED)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
  
//  if(update_texture->has_mips)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  
  // Put packing back to what it was.
  if(update_texture->format == GL_RED)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  }
  
  // Calling code checks for error.
}



} // ns