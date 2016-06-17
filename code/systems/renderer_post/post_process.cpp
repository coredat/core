#include <systems/renderer_post/post_process.hpp>
#include <graphics_api/mesh.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>


namespace
{
  Ogl::Vertex_buffer post_vbo;
  Ogl::Vertex_format post_vertex_format;
}



namespace Post_renderer {


void
initialize()
{
  // Vertex attr
  if(!Ogl::vertex_format_is_valid(&post_vertex_format))
  {
    constexpr uint32_t number_of_attrs = 2;
    const Ogl::Attribute_desc vert_desc[number_of_attrs]
    {
      Ogl::Attribute_desc{"in_vs_position",       Ogl::Attr_type::FLOAT2},
      Ogl::Attribute_desc{"in_vs_texture_coord",  Ogl::Attr_type::FLOAT2},
    };
    
    Ogl::vertex_format_load(&post_vertex_format,
                            vert_desc,
                            number_of_attrs);
  }
  
  // Fullscreen quad.
  if(!Ogl::vertex_buffer_is_valid(post_vbo))
  {
    constexpr float data[] = {
      -1.f, +1.f, 0.f, 1.f,
      +2.f, +1.f, 1.f, 0.f,
      -1.f, -2.f, 0.f, 1.f,
    };
    
    Ogl::vertex_buffer_load(&post_vbo, data, sizeof(data), 5 * 3, false);
  }
}



void
render()
{
}


} // ns