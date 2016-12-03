#include "ogl_texture_filtering.hpp"
#include "ogl_dimentions.hpp"


namespace Ogl {


/*
  TODO:
  - What if we are setting a texture that doens't have mips?
  - What if we don't support Anisotropy, or level 4?
*/
void
filtering_apply(const Graphics_api::Texture_filtering settings,
                const Graphics_api::Dimention dimentions)
{
  namespace Gfx = Graphics_api;
  
  const GLenum tex_dimention = Ogl::dimention_to_gl_texture_dimention(dimentions);
  
  const auto wrap_s = settings.wrap_mode_s == Gfx::Wrap_mode::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
  glTexParameteri(tex_dimention, GL_TEXTURE_WRAP_S, wrap_s);
  
  if(dimentions != Graphics_api::Dimention::one)
  {  
    const auto wrap_t = settings.wrap_mode_t == Gfx::Wrap_mode::clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    glTexParameteri(tex_dimention, GL_TEXTURE_WRAP_S, wrap_t);
  }
  
  switch(settings.filtering)
  {
    case(Gfx::Filtering_mode::point):
      glTexParameteri(tex_dimention, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(tex_dimention, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      break;
    case(Gfx::Filtering_mode::bilinear):
      glTexParameteri(tex_dimention, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(tex_dimention, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case(Gfx::Filtering_mode::trilinear):
      glTexParameteri(tex_dimention, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(tex_dimention, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      break;
    case(Gfx::Filtering_mode::anisotropic):
      glTexParameteri(tex_dimention, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
      break;
  }
}


} // ns