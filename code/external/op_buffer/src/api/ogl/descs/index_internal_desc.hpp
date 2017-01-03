#ifdef OP_BUFFER_API_OGL
#ifndef INDEX_INTERNAL_DESC_INCLUDED_3DB02FF8_EABB_4D35_A240_723F481932C1
#define INDEX_INTERNAL_DESC_INCLUDED_3DB02FF8_EABB_4D35_A240_723F481932C1


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct index_internal_desc
{
  GLuint    index_buffer_id  = 0;
  uint32_t  element_count = 0;
};


} // ns
} // ns


#endif // inc guard
#endif // api guard
