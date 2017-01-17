#ifdef OP_BUFFER_API_OGL
#ifndef VERTEX_FORMAT_DESC_A5BE5549_0328_44BA_817A_B42910763626
#define VERTEX_FORMAT_DESC_A5BE5549_0328_44BA_817A_B42910763626


#include "../details/config.hpp"
#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct attr_desc
{
  GLchar        name[max_attr_name_length()];
  GLenum        type    = GL_NONE;
  uint32_t      size    = 0;
  uintptr_t     pointer = 0;
};


struct vertex_format_desc
{
  attr_desc   attributes[max_vertex_attrs()];
  uint32_t    stride               = 0;
  uint32_t    number_of_attributes = 0;
  uint32_t    number_of_elements   = 0;
};


} // ns
} // ns


#endif // inc guard
#endif // ogl guard
