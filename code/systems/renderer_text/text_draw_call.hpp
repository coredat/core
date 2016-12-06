#ifndef DRAW_CALL_INCLUDED_A021FB15_A654_4DDF_8032_6E52F9FFCF85
#define DRAW_CALL_INCLUDED_A021FB15_A654_4DDF_8032_6E52F9FFCF85


#include <stdint.h>
#include <graphics_api/mesh.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>


namespace Text_renderer {


struct Draw_call
{
  float               world_matrix[16];
  Ogl::Texture        texture;
  Ogl::Texture        glyph_metrics;
  Ogl::Texture        string_info;
  uint32_t            string_size;
  uint32_t            cull_mask = -1;
};

} // ns


#endif // inc guard