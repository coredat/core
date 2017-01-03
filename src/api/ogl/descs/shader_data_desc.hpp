#ifdef OP_BUFFER_API_OGL
#ifndef SHADER_DATA_INCLUDED_404FD3C8_0C3A_47E8_9AE5_8C3EC7338DF4
#define SHADER_DATA_INCLUDED_404FD3C8_0C3A_47E8_9AE5_8C3EC7338DF4


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct shader_data_desc
{
  GLint  program;
  GLint  count;
  GLint  index;
  GLenum type;
};


} // ns
} // ns


#endif // inc guard
#endif // api guard
