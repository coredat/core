#ifndef VERTEX_FORMAT_INCLUDED_B30DDFF6_01C8_4CDA_8951_E30A4349AC86
#define VERTEX_FORMAT_INCLUDED_B30DDFF6_01C8_4CDA_8951_E30A4349AC86


#include "gfx_common.hpp"
#include "ogl/ogl_vertex_format.hpp"
#include <stdint.h>


namespace Graphics_api {


struct Vertex_format : public API_TYPE::Vertex_format
{
};


enum class Vertex_attribute
{
  position_3d,
  normal,
  texture_coord,
};


Vertex_format
vertex_format_create(const Vertex_attribute desc[],
                     const uint32_t attribute_count);
  


} // ns


#endif // inc guard