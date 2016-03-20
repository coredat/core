#ifndef OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB
#define OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB


#include "ogl_common.hpp"
#include "../pixel_format.hpp"
#include <assert.h>


namespace Ogl {


inline GLenum
pixel_format_get_gl_internal_format(const Graphics_api::Pixel_format format)
{
  namespace gfx = Graphics_api;

  switch(format)
  {
  case(gfx::Pixel_format::rgba8):
  case(gfx::Pixel_format::unknown):
    return GL_RGBA;
  case(gfx::Pixel_format::rgba32f):
    return GL_RGBA32F;
  
  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }
}


inline GLenum
pixel_format_get_format(const GLenum format)
{
  switch(format)
  {
  case(GL_RGBA):
  case(GL_RGBA32F):
    return GL_RGBA;

  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }
}


inline GLenum
pixel_format_get_type(const GLenum format)
{
  switch(format)
  {
  case(GL_RGBA):
    return GL_UNSIGNED_BYTE;
    
  case(GL_RGBA32F):
    return GL_FLOAT;

  default:
    assert(false); // Yikes.
    return GL_UNSIGNED_BYTE;
  }
}


}


#endif // inc guard