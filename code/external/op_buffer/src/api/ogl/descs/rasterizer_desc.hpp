#ifdef OP_BUFFER_API_OGL
#ifndef RASTERIZER_DESC_INCLUDED_127615A1_39E1_4180_858D_3D82AFF6808B
#define RASTERIZER_DESC_INCLUDED_127615A1_39E1_4180_858D_3D82AFF6808B


#include "../details/ogl_common.hpp"


namespace op {
namespace ogl {


struct rasterizer_desc
{
  GLenum cull_face;
  GLenum winding_order;
  GLenum primitive;
};


} // ns
} // ns


#endif // inc guard
#endif // api guard
