#ifndef VERTEX_FORMAT_INCLUDED_B30DDFF6_01C8_4CDA_8951_E30A4349AC86
#define VERTEX_FORMAT_INCLUDED_B30DDFF6_01C8_4CDA_8951_E30A4349AC86


#include "gfx_common.hpp"
#include "ogl/ogl_vertex_format.hpp"
#include <stdint.h>


namespace Graphics_api {


struct Vertex_format
{
  API_TYPE::Vertex_format format;
};


enum class Vertex_attribute
{
  position_3d,
  position_2d,
  normal,
  texture_coord,
};


Vertex_format
vertex_format_create(const Vertex_attribute desc[],
                     const uint32_t attribute_count);


uint32_t
vertex_format_get_number_of_attributes(const Vertex_format *fmt);


constexpr uint32_t
vertex_format_get_max_number_attributes() { return 16; }


/*!
  Its upto the calling code to make sure the array is the correct size.
*/
void
vertex_format_get_desc(const Vertex_format *fmt,
                       Vertex_attribute *out_desc);
  

} // ns


#endif // inc guard