#ifdef OP_BUFFER_API_OGL
#ifndef SHADER_DESC_INCLUDED_DD13E509_7F96_4CBE_8A36_58A1DF5B42BC
#define SHADER_DESC_INCLUDED_DD13E509_7F96_4CBE_8A36_58A1DF5B42BC


#include "../details/ogl_common.hpp"
#include "../details/config.hpp"


namespace op {
namespace ogl {


struct uniform_desc
{
  GLuint      program;
  char        name[max_uniform_name_length()];
  GLint       index;
  GLenum      type;
  GLint       count;
};


struct shader_desc
{
  GLuint vs;
  GLuint gs;
  GLuint ps;
  GLuint program;

  // Uniforms
  char         *uniform_names;
  uniform_desc *uniforms;
  size_t        uniform_count;
};


} // ns
} // ns


#endif // inc guard
#endif // ogl guard
