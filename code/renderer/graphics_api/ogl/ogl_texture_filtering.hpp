#ifndef OGL_TEXTURE_FILTERING_INCLUDED_95030160_0168_419D_8767_D1E0AF71D513
#define OGL_TEXTURE_FILTERING_INCLUDED_95030160_0168_419D_8767_D1E0AF71D513


#include "../texture_filtering.hpp"
#include "ogl_common.hpp"
#include <stdint.h>


// Some of this should move up a level.


namespace Ogl {



constexpr void
filtering_apply(const Graphics_api::Texture_filtering settings)
{
  namespace gfx = Graphics_api;
  
  const auto wrap_s = settings.wrap_s == gfx::Wrap_mode::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  
  const auto wrap_t = settings.wrap_t == gfx::Wrap_mode::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  
  GLenum filtering = GL_LINEAR;
  
  switch(settings.filtering)
  {
    case(gfx::Filtering_mode::point):
      filtering = GL_NEAREST;
      break;
    case(gfx::Filtering_mode::bilinear):
      filtering = GL_LINEAR;
      break;
    case(gfx::Filtering_mode::trilinear):
      filtering = GL_LINEAR_MIPMAP_LINEAR;
      break;
  }
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

}


} // ns


#endif // inc guard