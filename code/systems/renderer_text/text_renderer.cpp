#include "text_renderer.hpp"
#include <graphics_api/ogl/ogl_shader.hpp>
#include <graphics_api/ogl/ogl_shader_uniform.hpp>
#include <graphics_api/ogl/ogl_vertex_format.hpp>
#include <graphics_api/vertex_format.hpp>
#include <graphics_api/texture_filtering.hpp>
#include <graphics_api/ogl/ogl_texture_filtering.hpp>
#include <systems/renderer_common/vertex_format.hpp>


namespace {

  Ogl::Shader text_shader;
  Graphics_api::Vertex_format vert_fmt;
  Ogl::Uniform wvp_uni;
  Ogl::Uniform texture_uni;
}


namespace Text_renderer {


void
initialize()
{
  const char *vs_shader = R"(
    #version 150 core

    in vec3 in_vs_position;
    in vec3 in_vs_normal;
    in vec2 in_vs_texture_coord;

    uniform mat4 uni_wvp_mat;
  
    out vec2 in_ps_texture_coord;

    void
    main()
    {
      gl_Position = uni_wvp_mat * vec4(in_vs_position, 1.0);
      in_ps_texture_coord = in_vs_texture_coord;
    }
  )";
  
  const char *ps_shader = R"(
    #version 150 core

    in vec2 in_ps_texture_coord;
  
    uniform sampler2D uni_map_01;
    uniform vec3 uni_color = vec3(1,0,0.2);
  
    out vec4 out_frag_color;
  
    void
    main()
    {
      vec4 tex_sample = texture(uni_map_01, in_ps_texture_coord);
      out_frag_color = vec4(uni_color, tex_sample.r);
    }
  )";
  
  Ogl::shader_create(&text_shader, vs_shader, nullptr, ps_shader);
  
  vert_fmt = Renderer_common::get_standard_vertex_format();
  
  Ogl::Shader_uniforms unis;
  Ogl::shader_uniforms_retrive(&unis, &text_shader);
  
  Ogl::shader_uniforms_get_uniform_index(&wvp_uni, &unis, "uni_wvp_mat");
  Ogl::shader_uniforms_get_uniform_index(&texture_uni, &unis, "uni_map_01");
  
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
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Ogl::shader_bind(&text_shader);
  
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    Ogl::vertex_buffer_bind(calls[i].mesh.vbo, &vert_fmt.format, &text_shader);
    
    // Move the filter selection into the material.
    static Graphics_api::Texture_filtering filter =
    {
      Graphics_api::Filtering_mode::anisotropic,
      Graphics_api::Wrap_mode::wrap,
      Graphics_api::Wrap_mode::wrap
    };
    
    Ogl::filtering_apply(filter);
    
    Ogl::shader_uniforms_apply(wvp_uni, (void*)&view_proj_mat);
    Ogl::shader_uniforms_apply(texture_uni, (void*)&calls[i].texture.texture_id);
    
    const GLsizei count = calls[i].mesh.vbo.number_of_entries / vert_fmt.format.number_of_attributes;
    glDrawArrays(GL_TRIANGLES, 0, count);
    
    ++draw_call_count;
  }
  
  return draw_call_count;
}


} // ns
