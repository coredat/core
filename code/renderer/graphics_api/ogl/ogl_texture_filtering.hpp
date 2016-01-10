#ifndef OGL_TEXTURE_FILTERING_INCLUDED_95030160_0168_419D_8767_D1E0AF71D513
#define OGL_TEXTURE_FILTERING_INCLUDED_95030160_0168_419D_8767_D1E0AF71D513


#include "../texture_filtering.hpp"
#include "ogl_common.hpp"
#include <stdint.h>


// Some of this should move up a level.


namespace Ogl {



inline void
filtering_apply(const Graphics_api::Texture_filtering settings)
{
  namespace Gfx = Graphics_api;
  
  const auto wrap_s = settings.wrap_s == Gfx::Wrap_mode::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  
  const auto wrap_t = settings.wrap_t == Gfx::Wrap_mode::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  
  GLenum filtering = GL_LINEAR;
  
  switch(settings.filtering)
  {
    case(Gfx::Filtering_mode::point):
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case(Gfx::Filtering_mode::bilinear):
      filtering = GL_LINEAR;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case(Gfx::Filtering_mode::trilinear):
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case(Gfx::Filtering_mode::anisotropic):
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
      break;
  }
}


} // ns


#endif // inc guard