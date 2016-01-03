#ifndef OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB
#define OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB


#include "ogl_common.hpp"
#include <assert.h>


namespace Ogl {


enum class Pixel_format
{
  rgba8,
  rgba32f,
};


inline GLenum
pixel_format_get_internal_format(const Pixel_format format)
{
  switch(format)
  {
  case(Pixel_format::rgba8):
    return GL_RGBA;
  case(Pixel_format::rgba32f):
    return GL_RGBA32F;
    break;

  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }
}


inline GLenum
pixel_format_get_format(const Pixel_format format)
{
  switch(format)
  {
  case(Pixel_format::rgba8):
  case(Pixel_format::rgba32f):
    return GL_RGBA;
    break;

  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }
}


inline GLenum
pixel_format_get_type(const Pixel_format format)
{
  switch(format)
  {
  case(Pixel_format::rgba8):
    return GL_UNSIGNED_BYTE;
    break;
    
  case(Pixel_format::rgba32f):
    return GL_FLOAT;
    break;

  default:
    assert(false); // Yikes.
    return GL_UNSIGNED_BYTE;
  }
}


}


#endif // inc guard