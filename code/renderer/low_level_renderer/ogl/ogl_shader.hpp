#ifndef OGL_SHADER_INCLUDED_4A57DD2B_3994_4F0A_9AC8_B9374A867208
#define OGL_SHADER_INCLUDED_4A57DD2B_3994_4F0A_9AC8_B9374A867208


#include "ogl_common.hpp"


namespace Ogl {


/*!
  A shader program.
*/
struct Shader
{
  GLuint program_id;
  GLuint vert_shader_id;
  GLuint geo_shader_id;
  GLuint frag_shader_id;
};


/*!
  Attempts to create a shader for rendering.
  \param out_shader The shader output.
  \param vert_shader_code The vertex shader code.
  \param geo_shader_code The geometry shader code, this is optional.
  \param frag_shader_code The fragment shader code.
  \param log Optional logging stream.
*/
void
shader_create(Shader *out_shader,
                     const char *vert_shader_code,
                     const char *geo_shader_code,
                     const char *frag_shader_code,
                     std::ostream *log = nullptr);

/*!
  Attempts to create a shader for rendering.
  \param shader_to_destroy The shader that will be destroyed.
  \param log Optional logging stream.
*/
void
shader_destroy(Shader *shader_to_destroy,
               std::ostream *log = nullptr);


} // ns


#endif // inc guard