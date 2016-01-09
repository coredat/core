#ifndef OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB
#define OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB


#include "ogl_common.hpp"
#include "../pixel_format.hpp"
#include <assert.h>


namespace Ogl {


constexpr GLenum
gl_pixel_format(const Low_level_renderer::Pixel_format format)
{
  namespace LLR = Low_level_renderer;

  switch(format)
  {
  case(LLR::Pixel_format::rgba8):
    return GL_RGBA;
  case(LLR::Pixel_format::rgba32f):
    return GL_RGBA32F;
    break;

  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }
}


constexpr GLenum
pixel_format_get_format(const GLenum format)
{
  switch(format)
  {
  case(GL_RGBA):
  case(GL_RGBA32F):
    return GL_RGBA;
    break;

  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }
}


constexpr GLenum
pixel_format_get_type(const GLenum format)
{
  switch(format)
  {
  case(GL_RGBA):
    return GL_UNSIGNED_BYTE;
    break;
    
  case(GL_RGBA32F):
    return GL_FLOAT;
    break;

  default:
    assert(false); // Yikes.
    return GL_UNSIGNED_BYTE;
  }
}


}


#endif // inc guard