#ifdef OP_BUFFER_API_OGL
#ifndef TEXTURE_DESC_INCLUDED_93756700_E853_4660_B593_3BC52537780D
#define TEXTURE_DESC_INCLUDED_93756700_E853_4660_B593_3BC52537780D


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct texture_desc
{
  GLuint  texture_id;
  GLsizei width;
  GLsizei height;
  GLsizei depth;
  GLenum  dimention;
  GLenum  format;
  GLenum  type;
  GLint   internal_format;
  bool    mips;

  GLint   filter_min;
  GLint   filter_mag;
  GLfloat filter_multiplier;

  GLint   wrap_s_coord;
  GLint   wrap_t_coord;
  GLint   wrap_r_coord;
};


} // ns
} // ns


#endif // inc guard
#endif // api guard
