#ifdef OP_BUFFER_API_OGL
#ifndef BLEND_INTERNAL_DESC_INCLUDED_D601B44B_34D3_4A90_B3A2_F37D9E410031
#define BLEND_INTERNAL_DESC_INCLUDED_D601B44B_34D3_4A90_B3A2_F37D9E410031


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct blend_internal_desc
{
  GLenum enabled;
  GLenum src_factor;
  GLenum dest_factor;
};


} // ns
} // ns


#endif // inc guard
#endif // api guard
