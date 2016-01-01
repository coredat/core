#ifndef OGL_SHADER_INCLUDED_4A57DD2B_3994_4F0A_9AC8_B9374A867208
#define OGL_SHADER_INCLUDED_4A57DD2B_3994_4F0A_9AC8_B9374A867208


#include "ogl_common.hpp"


namespace Ogl {


struct Shader
{
  GLuint program_id;
  GLuint vert_shader_id;
  GLuint geo_shader_id;
  GLuint frag_shader_id;
};


} // ns


#endif // inc guard