#ifndef OGL_DIMENTIONS_INCLUDED_668793E9_E972_4077_AA95_CE11C02CEBB1
#define OGL_DIMENTIONS_INCLUDED_668793E9_E972_4077_AA95_CE11C02CEBB1


#include "ogl_common.hpp"
#include "../dimention.hpp"
#include <utilities/optimizations.hpp>


namespace Ogl {


inline GLenum
dimention_to_gl_texture_dimention(const Graphics_api::Dimention dimentions)
{
  namespace gfx = Graphics_api;

  switch(dimentions)
  {
  case(gfx::Dimention::one):
    return GL_TEXTURE_1D;
  case(gfx::Dimention::two):
    return GL_TEXTURE_2D;
  case(gfx::Dimention::three):
    return GL_TEXTURE_3D;
  default:
    assert(false);
  }

  UNREACHABLE;
}


} // ns


#endif // inc guard