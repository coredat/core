#include <systems/renderer_material/material_renderer.hpp>
#include <graphics_api/texture_filtering.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>
#include <graphics_api/ogl/ogl_common.hpp>
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/utils/shader_utils.hpp>


namespace
{
  int32_t mat_renderer_last_program = -1;
  int32_t mat_renderer_last_ibo     = -1;
  int32_t mat_renderer_last_vbo     = -1;
  
  Ogl::Vertex_format mat_renderer_vertex_format;
}


namespace Material_renderer {


void
initialize()
{
  // Vertex attr
  {
    constexpr uint32_t number_of_attrs = 3;
    const Ogl::Attribute_desc vert_desc[number_of_attrs]
    {
      Ogl::Attribute_desc{"in_vs_position",       Ogl::Attr_type::FLOAT3},
      Ogl::Attribute_desc{"in_vs_texture_coord",  Ogl::Attr_type::FLOAT2},
      Ogl::Attribute_desc{"in_vs_normal",         Ogl::Attr_type::FLOAT3},
    };
    
    Ogl::vertex_format_load(&mat_renderer_vertex_format,
                            vert_desc,
                            number_of_attrs);
  }
}


void
reset()
{
  mat_renderer_last_ibo     = -1;
  mat_renderer_last_vbo     = -1;
  mat_renderer_last_program = -1;
}


void
render(const float view_proj_mat[16],
       const Material material,
       const Draw_call calls[],
       const uint32_t number_of_calls)
{
  // Bind material.
  
  // For all draw calls
}


} // ns