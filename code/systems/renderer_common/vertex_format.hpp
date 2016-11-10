#ifndef RENDERER_COMMON_INCLUDED_C6B32025_F808_4E39_AA96_F89B120E4709
#define RENDERER_COMMON_INCLUDED_C6B32025_F808_4E39_AA96_F89B120E4709


#include <graphics_api/vertex_format.hpp>


namespace Renderer_common {

constexpr unsigned int vertex_attr_count = 3;

constexpr Graphics_api::Vertex_attribute vertex_desc[vertex_attr_count] =
{
  Graphics_api::Vertex_attribute::position_3d,
  Graphics_api::Vertex_attribute::texture_coord,
  Graphics_api::Vertex_attribute::normal,  
};


/*!
  Standard vertex format for most mesh style renderings.
*/
Graphics_api::Vertex_format
get_standard_vertex_format()
{
  static Graphics_api::Vertex_format vert_fmt;
  
  if(!Graphics_api::vertex_format_is_valid(&vert_fmt))
  {
    vert_fmt = Graphics_api::vertex_format_create(vertex_desc, vertex_attr_count);
  }
  
  return vert_fmt;
}

} // ns


#endif // inc guard