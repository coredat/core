#ifndef GEOMETRY_INCLUDED_8B13A7B5_F66D_4B1A_9F15_A64897DDB737
#define GEOMETRY_INCLUDED_8B13A7B5_F66D_4B1A_9F15_A64897DDB737


#include <graphics_api/gfx_fwd.hpp>
#include <stddef.h>


namespace Graphics_api {

struct Quad_info
{
  float position[3];
  float normal[3];
  float uv[2];
  float st[2];
};



Mesh
create_quads(const Vertex_format *fmt,
             const Quad_info *quad_info,
             const size_t number_of_quads);


} // ns


#endif // inc guard