#ifndef SIMPLE_RENDERER_NODE_INCLUDED_849FA332_002C_40E6_BB13_BD4879151CE3
#define SIMPLE_RENDERER_NODE_INCLUDED_849FA332_002C_40E6_BB13_BD4879151CE3


#include <stdint.h>
#include <renderer/graphics_api/ogl/ogl_vertex_buffer.hpp>
#include <renderer/graphics_api/ogl/ogl_texture.hpp>


namespace Simple_renderer {


struct Node
{
  float                     wvp[16];
  float                     world_mat[16];
  Ogl::Texture              diffuse;
  Ogl::Vertex_buffer        vbo;
  uint32_t                  number_of_verts;
};


} // ns


#endif // inc guard