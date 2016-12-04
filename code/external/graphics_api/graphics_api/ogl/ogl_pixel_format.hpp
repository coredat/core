#ifndef OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB
#define OGL_PIXEL_FORMAT_INCLUDED_C8DE0E25_B8D7_4C24_B957_D70AC8AFBDFB


#include "ogl_common.hpp"
#include "../pixel_format.hpp"
#include <utilities/optimizations.hpp>


namespace Ogl {


inline GLenum
pixel_format_get_gl_internal_format(const Graphics_api::Pixel_format format)
{
  namespace gfx = Graphics_api;

  switch(format)
  {
  case(gfx::Pixel_format::rgb8):
    return GL_RGB;
  case(gfx::Pixel_format::rgba8):
  case(gfx::Pixel_format::unknown):
    return GL_RGBA;
  case(gfx::Pixel_format::rgba32f):
    return GL_RGBA32F;
  case(gfx::Pixel_format::r32f):
    return GL_R32F;
  
  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }

  UNREACHABLE;
}


inline Graphics_api::Pixel_format
pixel_format_get_gfx_api_format(const GLenum format)
{
  namespace gfx = Graphics_api;

  switch(format)
  {
  case(GL_RGB):
    return gfx::Pixel_format::rgb8;
  case(GL_RGBA):
    return gfx::Pixel_format::rgba8;
  case(GL_R32F):
    return gfx::Pixel_format::r32f;
  case(GL_RGBA32F):
    return gfx::Pixel_format::rgba32f;
  
  default:
    assert(false); // Yikes.
    return gfx::Pixel_format::unknown;
  }

  UNREACHABLE;
}


inline GLenum
pixel_format_get_format(const GLenum format)
{
  switch(format)
  {
  case(GL_RED):
  case(GL_R32F):
  case(GL_DEPTH24_STENCIL8): // unsure about this.
    return GL_RED;
  
  case(GL_RGB):
  case(GL_RGB32F):
    return GL_RGB;
  
  case(GL_RGBA):
  case(GL_RGBA32F):
    return GL_RGBA;

  default:
    assert(false); // Yikes.
    return GL_RGBA;
  }

  UNREACHABLE;
}


inline GLenum
pixel_format_get_type(const GLenum format)
{
  switch(format)
  {
  case(GL_RED):
  case(GL_RGB):
  case(GL_RGBA):
  case(GL_DEPTH24_STENCIL8): // unsure about this.
    return GL_UNSIGNED_BYTE;

  case(GL_R32F):
  case(GL_RGB32F):
  case(GL_RGBA32F):
    return GL_FLOAT;

  default:
    assert(false); // Yikes.
    return GL_UNSIGNED_BYTE;
  }

  UNREACHABLE;
}


inline const char*
pixel_format_get_name(const GLenum format)
{
  switch(format)
  {
  case(GL_RED):              return "GL_RED";
  case(GL_RGB):              return "GL_RGB";
  case(GL_RGBA):             return "GL_RGBA";
  case(GL_DEPTH24_STENCIL8): return "GL_DEPTH_24_STENCIL8";
  case(GL_R32F):             return "GL_R32F";
  case(GL_RGBA32F):          return "GL_RGBA32F";

  default:
    assert(false); // Yikes.
    return "Unknown";
  }

  UNREACHABLE;
}


}


#endif // inc guard
