#ifdef OP_BUFFER_API_OGL
#ifndef GEOMETRY_DESC_INCLUDED_319AA656_8002_4441_8272_20D7B0F6C7C9
#define GEOMETRY_DESC_INCLUDED_319AA656_8002_4441_8272_20D7B0F6C7C9


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct geometry_desc
{
  GLuint    vertex_buffer_id  = 0;
  uint32_t  element_count = 0;
};


} // ns
} // ns


#endif // inc guard
#endif // ogl guard
