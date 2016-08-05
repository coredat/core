#include "text_renderer.hpp"
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/vertex_format.hpp>


namespace {

  Ogl::Shader text_shader;
  Graphics_api::Vertex_format vert_fmt;
  Ogl::Uniform wvp_uni;

}


namespace Text_renderer {


void
initialize()
{
  const char *vs_shader = R"(
    #version 150 core

    in vec3 vs_in_position;
    in vec3 vs_in_normal;
    in vec2 vs_in_texture_coords;

    uniform mat4 uni_wvp_mat;
  
    out vec2 ps_in_texture_coords;

    void
    main()
    {
      gl_Position = uni_wvp_mat * vec4(vs_in_position, 1.0);
    }
  )";
  
  const char *ps_shader = R"(
    #version 150 core

    in vec2 in_ps_texture_coord;
  
    uniform sampler2D uni_map_01;
  
    out vec4 out_frag_color;
  
    void
    main()
    {
//      vec4 tex_sample = texture(uni_map_01, in_ps_texture_coord);
//      out_frag_color = tex_sample.rgba;
      
      out_frag_color = vec4(1,1,1,1);
    }
  )";
  
  Ogl::shader_create(&text_shader, vs_shader, nullptr, ps_shader);
  
  const Graphics_api::Vertex_attribute vertex_desc[] = {
    Graphics_api::Vertex_attribute::position_3d,
    Graphics_api::Vertex_attribute::normal,
    Graphics_api::Vertex_attribute::texture_coord,
  };
  
  vert_fmt = Graphics_api::vertex_format_create(vertex_desc, 3);
  
  Ogl::Shader_uniforms unis;
  Ogl::shader_uniforms_retrive(&unis, &text_shader);
  
  Ogl::shader_uniforms_get_uniform_index(&wvp_uni, &unis, "uni_wvp_mat");
  
}

void
reset()
{
}


uint32_t
render(const math::mat4 &view_proj_mat,
       const Draw_call calls[],
       const uint32_t number_of_calls)
{
  uint32_t draw_call_count = 0;

  Ogl::shader_bind(&text_shader);
  
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
  
    Ogl::vertex_buffer_bind(calls[i].mesh.vbo, &vert_fmt.format, &text_shader);
    
    Ogl::shader_uniforms_apply(wvp_uni, (void*)&view_proj_mat);
    
    const GLsizei count = calls[i].mesh.vbo.number_of_entries / vert_fmt.format.number_of_attributes;
    glDrawArrays(GL_TRIANGLES, 0, count);
    
    ++draw_call_count;
  }
  
  return draw_call_count;
}


} // ns
