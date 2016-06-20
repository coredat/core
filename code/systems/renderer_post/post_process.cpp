#include <systems/renderer_post/post_process.hpp>
#include <graphics_api/mesh.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>


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
    constexpr float data[] =
    {
      -1.f, +1.f, 0.f, 1.f,
      -1.f, -1.f, 0.f, 0.f,
      +1.f, +1.f, 1.f, 1.f,
      
      -1.f, -1.f, 0.f, 0.f,
      +1.f, -1.f, 1.f, 0.f,
      +1.f, +1.f, 1.f, 1.f,
    };
    
    Ogl::vertex_buffer_load(&post_vbo, data, sizeof(data), 5 * 3, false);
  }
}



void
render(Post_renderer::Post_shader *shd)
{
//  glDisable(GL_CULL_FACE);
  // Bind the shader
  Ogl::shader_bind(&shd->shader);
  Ogl::vertex_buffer_bind(post_vbo, &post_vertex_format, &shd->shader);

  {
    // Move the filter selection into the material.
    static Graphics_api::Texture_filtering filter =
    {
      Graphics_api::Filtering_mode::anisotropic,
      Graphics_api::Wrap_mode::wrap,
      Graphics_api::Wrap_mode::wrap
    };
    
    Ogl::filtering_apply(filter);
    if(shd->map_01_id.texture_id)
      Ogl::shader_uniforms_apply(shd->map_01, (void*)&shd->map_01_id.texture_id);
    
    if(shd->map_02_id.texture_id)
      Ogl::shader_uniforms_apply(shd->map_02, (void*)&shd->map_02_id.texture_id);
    
    if(shd->map_03_id.texture_id)
      Ogl::shader_uniforms_apply(shd->map_03, (void*)&shd->map_03_id.texture_id);
    
    if(shd->current_time_uni.index > -1)
    {
      Ogl::shader_uniforms_apply(shd->current_time_uni, (void*)&shd->current_time_uni);
    }
    
    if(shd->delta_time_uni.index > -1)
    {
      Ogl::shader_uniforms_apply(shd->delta_time_uni, (void*)&shd->delta_time);
    }
  }
  
  glDrawArrays(GL_TRIANGLES, 0, 6);
}


} // ns